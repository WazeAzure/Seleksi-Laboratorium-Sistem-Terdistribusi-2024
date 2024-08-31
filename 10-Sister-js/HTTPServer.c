#include "HTTPServer.h"
#include "Systems/ThreadPool.h"
#include "HTTPResponse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include <signal.h>

void launch(struct HTTPServer *);
void register_routes(struct HTTPServer *server, char * (*route_function)(struct HTTPServer *server, struct HTTPRequest *request), char *uri, int num_methods, ...);
void *handler(void *arg);


// Global variable to hold the server socket descriptor
int server_socket;

// Signal handler function
void handle_signal(int signal) {
    if (signal == SIGINT) {
        printf("\nShutting down the server...\n");
        close(server_socket); // Close the server socket
        _exit(0); // Exit the program
    }
}

struct ClientServer {
    int client;
    struct HTTPServer *server;
};

struct Route {
    int methods[9];
    int neff;
    char *uri;
    char *(*route_functions)(struct HTTPServer *server, struct HTTPRequest *request);
};

struct HTTPServer http_server_constructor(int port)
{
    struct HTTPServer server;
    server.server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, 255);
    server.routes = dictionary_constructor(compare_string_keys);
    server.register_routes = register_routes;
    server.launch = launch;

    return server;
}

enum HTTPMethods str2enum(char *string){
    if(!strcmp(string, "GET")){
        return GET;
    }
    else if(!strcmp(string, "POST")){
        return POST;
    }
    else if(!strcmp(string, "PUT")){
        return PUT;
    }
    else if(!strcmp(string, "DELETE")){
        return DELETE;
    }
    return NONE;
}

int method_allowed_checker(enum HTTPMethods req, struct Route *route){
    for(int i=0; i<route->neff; i++){
        if(route->methods[i] == req){
            return 1;
        }
    }
    return 0;
}

void register_routes(struct HTTPServer *server, char *(*route_function)(struct HTTPServer *server, struct HTTPRequest *request), char *uri, int num_methods, ...){
    struct Route route;
    va_list methods;
    va_start(methods, num_methods);

    for(int i=0; i<num_methods; i++){
        route.methods[i] = va_arg(methods, int);
    }

    strcpy(route.uri, uri);
    route.route_functions = route_function;
    route.neff = num_methods;

    server->routes.insert(&server->routes, uri, sizeof(char[strlen(uri)]), &route, sizeof(route));
}

void launch(struct HTTPServer *server){
    struct ThreadPool thread_pool = thread_pool_constructor(2);
    struct sockaddr *sock_addr = (struct sockaddr*)&server->server.address;
    socklen_t address_length = (socklen_t)sizeof(server->server.address);
    
    signal(SIGINT, handle_signal);
    signal(SIGSEGV, handle_signal);

    while(1){
        printf("======== WAITING FOR REQUEST ========\n");

        struct ClientServer *client_server = malloc(sizeof(struct ClientServer));
        memset(client_server, 0, sizeof(struct ClientServer));
        fork();
        client_server->client = accept(server->server.socket, sock_addr, &address_length);
        client_server->server = server;

        // struct ThreadJob job = thread_job_constructor(handler, client_server);
        // thread_pool.add_work(&thread_pool, job);
        // struct ThreadJob job = thread_job_constructor(handler, client_server);
        
        handler(client_server);
        // thread_pool.add_work(&thread_pool, job);
    }
}

void *handler(void *arg){
    
    struct ClientServer *client_server = (struct ClientServer *)arg;
    
    char *request_string = malloc(50000);
    memset(request_string, 0, 50000);
    read(client_server->client, request_string, 50000);

    struct HTTPRequest request = http_request_constructor(request_string);
    char *uri = request.request_line.search(&request.request_line, "uri", sizeof("uri"));

    char *query_start = strchr(uri, '?');  // Find the first occurrence of '?'
    
    if (query_start) {
        *query_start = '\0';  // Truncate the string at the '?'
    }

    struct Route *route = client_server->server->routes.search(&client_server->server->routes, uri, sizeof(char[strlen(uri)]));

    char* method_request = malloc(10);
    memset(method_request, 0, 10);
    method_request = request.request_line.search(&request.request_line, "method", sizeof("method"));
    enum HTTPMethods x = str2enum(method_request);

    free(method_request);
    
    if(!method_allowed_checker(x, route)){
        // un allowed method
        char *response = "HTTP/1.1 405 Method Not Allowed\r\nServer: Apache/2.2.14 (Win32)\r\nConnection:Closed\r\n";
        write(client_server->client, response, strlen(response));
        close(client_server->client);
        free(client_server);
        return NULL;
    }

    char *response = route->route_functions(client_server->server, &request);



    // char *header_ok = "HTTP/1.1 200 OK\r\nServer: Apache/2.2.14 (Win32)\r\nContent-Type: text/html\r\nConnection:Closed\r\n\r\n";

    // char *baru = malloc(strlen(header_ok) + strlen(response));
    // strcpy(baru, header_ok);
    // strcat(baru, response);

    // create per case
    // struct HTTPResponse *structured_response = create_response(200, (char *)request.header_fields.search(&request.header_fields, "Accept", sizeof("Accept")));
    // char *baru = parse_response_struct(structured_response, response);

    write(client_server->client, response, sizeof(char[strlen(response)]));
    close(client_server->client);
    free(response);
    free(request_string);
    // free(structured_response);

    free(client_server);
    return NULL;
}

char *render_template(char *filepath, struct Dictionary *dictionary){
    // Open the file
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Could not open file");
        return NULL;
    }
    
    // Create a buffer to store the file content
    char *buffer = malloc(30000);  // Arbitrary size, you may want to adjust this
    memset(buffer, 0, 30000);
    
    // Temporary buffer for reading each line
    char line[1024];  // Assuming no line exceeds 1024 characters
    int buffer_position = 0;

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        char *start = line;
        char *placeholder_start;
        
        // Find the start of the placeholder `${`
        while ((placeholder_start = strstr(start, "${")) != NULL) {
            // Copy everything before the placeholder to the buffer
            int prefix_len = placeholder_start - start;
            memcpy(&buffer[buffer_position], start, prefix_len);
            buffer_position += prefix_len;
            
            // Find the end of the placeholder `}`
            char *placeholder_end = strchr(placeholder_start, '}');
            if (placeholder_end != NULL) {
                // Extract the placeholder key (e.g., "name")
                int key_len = placeholder_end - placeholder_start - 2;
                char key[key_len + 1];
                strncpy(key, placeholder_start + 2, key_len);
                key[key_len] = '\0';
                
                // Lookup the key in the dictionary
                char *value = dictionary->search(dictionary, key, strlen(key) + 1);
                
                // If a value was found, replace the placeholder with the value
                if (value != NULL) {
                    int value_len = strlen(value);
                    memcpy(&buffer[buffer_position], value, value_len);
                    buffer_position += value_len;
                }
                
                // Move the start pointer to the character after the closing `}`
                start = placeholder_end + 1;
            } else {
                // If no closing `}`, copy the rest of the line as is
                break;
            }
        }
        
        // Copy any remaining part of the line after the last placeholder
        int remaining_len = strlen(start);
        memcpy(&buffer[buffer_position], start, remaining_len);
        buffer_position += remaining_len;
    }
    
    // Close the file
    fclose(file);
    
    // Return the rendered content
    return buffer;
}