#ifndef HTTPRESPONSE
#define HTTPRESPONSE

struct HTTPResponse {
    int status_code;
    
    char content_type[100];
};

char *parse_response_struct(struct HTTPResponse *response, void *body);

struct HTTPResponse *create_response(int status_code, char *accept);

#endif