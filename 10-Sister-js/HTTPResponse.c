#include "HTTPResponse.h"
#include "DataStructures/Dictionary/Dictionary.h"
#include "JsonParser.h"
#include <string.h>
#include <stdlib.h>

struct HTTPResponse *create_response(int status_code, char *accept){
    struct HTTPResponse *response = (struct HTTPResponse*)malloc(sizeof(struct HTTPResponse));
    memset(response, 0, sizeof(struct HTTPResponse));
    response->status_code = status_code;
    if(accept){
        strcpy(response->content_type, accept);
    }

    return response;
}

char *parse_response_struct(struct HTTPResponse *response, void *body){
    char *head = "HTTP/1.1 ";
    char status_code[30]; 
    if(response->status_code == 200) {
        strcpy(status_code, "200 OK\r\n");
    }

    char *server = "Server: Apache/2.2.14 (Win32)\r\n";
    char *content_type = response->content_type;

    char *connection = "Connection: Closed\r\n\r\n";
    // TODO: add response
    size_t resp_total_length = strlen(body) + strlen(head) + strlen(status_code) + strlen(server) + strlen(content_type) + strlen(connection) + strlen("Content-Type: ") + 4;
    char *response_final = malloc(resp_total_length);
    memset(response_final, 0, resp_total_length);
    strcpy(response_final, head);
    strcat(response_final, status_code);
    strcat(response_final, "Content-Type: ");
    strcat(response_final, content_type);
    strcat(response_final, "\r\n");
    strcat(response_final, server);
    strcat(response_final, connection);

    if(strstr(content_type, "text/plain")){
        strcat(response_final, (char*)body);
    }
    else if(strstr(content_type, "application/json")){
        struct Dictionary* new_body = malloc(sizeof(struct Dictionary));
        memset(new_body, 0, sizeof(struct Dictionary));
        new_body = (struct Dictionary*)body;
        char *json_res = dictionary_to_json(new_body);
        strcat(response_final, json_res);
    }
    else {
        strcat(response_final, body);
    }

    return response_final;
}