#include "../HTTPServer.h"
#include "../HTTPResponse.h"
#include "../Systems/Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *root_folder = "/mnt/c/Users/Asus Tuf Gaming/Documents/Github Desktop/Seleksi-Laboratorium-Sistem-Terdistribusi-2024/10-Sister-js/test_server/";

char *template(struct HTTPServer *server, struct HTTPRequest *request){
    char index[3000] = {0};

    strcpy(index, root_folder);

    strcat(index, "template.html");

    /* CARA PERTAMA bisa manual */
    struct Dictionary var_to_substitute;
    // var_to_substitute = dictionary_constructor(compare_string_keys);
    // var_to_substitute.insert(&var_to_substitute, "name", strlen("name"), "edbert", strlen("edbert"));
    // var_to_substitute.insert(&var_to_substitute, "gender", strlen("gender") , "boy", strlen("boy"));

    /* CARA KEDUA ambil dari request param langsung */
    var_to_substitute = request->query_params;

    // get files needed
    char *body = render_template(index, &var_to_substitute);

    // char *header = "HTTP/1.1 200 OK\r\nServer: Apache/2.2.14 (Win32)\r\nConnection:Closed\r\n";

    // create final response
    char *accept = request->header_fields.search(&request->header_fields, "Accept", strlen("Accept"));
    struct HTTPResponse *final_response = create_response(200, accept);

    char *x = parse_response_struct(final_response, body);

    return x;
}

char *leak_dict(struct HTTPServer *server, struct HTTPRequest *request){
    char *accept = request->header_fields.search(&request->header_fields, "Accept", strlen("Accept"));
    struct HTTPResponse *final_response = create_response(200, accept);

    char *x = parse_response_struct(final_response, (void *)&request->query_params);
    return x;
}

char *home(struct HTTPServer *server, struct HTTPRequest *requst){
    int temp_x = strlen(root_folder);
    char index[3000] = {0};
    strcpy(index, root_folder);
    strcat(index, "index.html");

    // char *s = requst->query_params.search(&requst->query_params, "id", strlen("id"));
    // printf("hasil id: %s\n", s);
    // char *x = render_template(1, index);
    char *x = "HTTP/1.1 200 OK\r\nServer: Apache/2.2.14 (Win32)\r\nConnection:Closed\r\n\r\n";
    return x;
}

char *delete_test(struct HTTPServer *server, struct HTTPRequest *requst){
    int temp_x = strlen(root_folder);

    char *filename = requst->query_params.search(&requst->query_params, "file", strlen("file"));
    char file_delete[3000];

    strcpy(file_delete, root_folder);
    strcat(file_delete, filename);

    // remove file
    if(remove(file_delete) == 0){
        printf("delete success!\n");
    } else {
        printf("delete failed!\n");
    }

    char template[100] = "HTTP/1.1 200 OK\r\nServer: Apache/2.2.14 (Win32)\r\nConnection:Closed\r\n\r\ndelete succeed";
    char *x = malloc(strlen(template)+1);
    memset(x, 0, strlen(template)+1);
    strcpy(x, template);
    return x;
}

char *put_test(struct HTTPServer *server, struct HTTPRequest *requst){
    char *filename = requst->query_params.search(&requst->query_params, "file", strlen("file"));

    char *path = malloc(strlen(root_folder) + strlen(filename) + 1);
    memset(path, 0, sizeof(strlen(root_folder) + strlen(filename) + 1));
    strcpy(path, root_folder);
    strcat(path, filename);


    char *data;
    if(requst->body.keys.head != 0){
        data = requst->body.search(&requst->body, "name", strlen("name"));
    } else {
        char *temp = malloc(strlen("sudah lelah") + 1);
        memset(temp, 0, strlen("sudah lelah")+1);
        strcpy(temp, "sudah lelah");
        
        data = temp;
    }

    append_file(path, data, strlen(data));

    char template[100] = "HTTP/1.1 200 OK\r\nServer: Apache/2.2.14 (Win32)\r\nConnection:Closed\r\n\r\niput succeed";
    char *x = malloc(strlen(template));
    memset(x, 0, strlen(template));
    strcpy(x, template);

    return x;
}

char *post_save_file(struct HTTPServer *server, struct HTTPRequest *requst){
    // save file
    
    char *filename = requst->query_params.search(&requst->query_params, "file", strlen("file"));

    char *path = malloc(strlen(root_folder) + strlen(filename) + 1);
    memset(path, 0, strlen(root_folder) + strlen(filename) + 1);
    strcpy(path, root_folder);
    strcat(path, filename);

    if(requst->body.keys.head != 0){
        char *data = requst->body.search(&requst->body, "name", strlen("name"));
        if(data == 0x0){
            data = requst->body.search(&requst->body, "data", strlen("data"));
        }
        write_file(path, data, strlen(data));
    }

    char template[100] = "HTTP/1.1 200 OK\r\nServer: Apache/2.2.14 (Win32)\r\nConnection:Closed\r\n\r\nsave file succeed";
    char *x = malloc(strlen(template));
    memset(x, 0, strlen(template));
    strcpy(x, template);

    return x;
}

int main(){
    const int PORT = 1237;
    struct HTTPServer server = http_server_constructor(PORT); // input PORT

    server.register_routes(&server, template, "/template", 1, GET);
    server.register_routes(&server, home, "/", 1, GET);
    
    server.register_routes(&server, leak_dict, "/leak_dict", 1, GET);

    // CATATN DELETE: pake query params untuk nerima file nya >:V
    server.register_routes(&server, delete_test, "/del", 1, DELETE);

    // CATATAN PUT: ga ada si :V
    server.register_routes(&server, put_test, "/put", 1, PUT);

    // CATATAN: gunakan query params untuk nama file >:V
    server.register_routes(&server, post_save_file, "/save-file", 1, POST);

    server.launch(&server);

    return 0;
}