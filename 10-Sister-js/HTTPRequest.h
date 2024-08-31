#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include <stdio.h>
#include "./DataStructures/Dictionary/Dictionary.h"

struct HTTPRequest {
    struct Dictionary request_line;
    struct Dictionary header_fields;
    struct Dictionary query_params;
    struct Dictionary body;
};

struct HTTPRequest http_request_constructor(char *request_string);
void http_request_destructor(struct HTTPRequest *request);

#endif