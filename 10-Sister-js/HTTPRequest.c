#include "HTTPRequest.h"
#include <string.h>
#include <stdlib.h>
#include "./DataStructures/Lists/Queue.h"
#include "JsonParser.h"

void extract_request_line_fields(struct HTTPRequest *request, char *request_line);
void extract_header_fields(struct HTTPRequest *request, char *header_fields);
void extract_body(struct HTTPRequest *request, char *body);


struct HTTPRequest http_request_constructor(char *request_string) {
    
    for(int i=0; i<strlen(request_string) -1; i++){
        if(request_string[i] == '\n'){
            if(request_string[i+1] == '\n') request_string[i+1] = '|';
            else if(request_string[i+2] == '\n') request_string[i+1] = '|';
        }
    }
    
    char *request_line = strtok(request_string, "\n");
    char *header_fields = strtok(NULL, "|");
    
    char *body = strtok(NULL, "|");

    struct HTTPRequest request;

    extract_request_line_fields(&request, request_line);
    extract_header_fields(&request, header_fields);
    extract_body(&request, body);
    
    return request;
}

void http_request_destructor(struct HTTPRequest *request)
{
    dictionary_destructor(&request->request_line);
    dictionary_destructor(&request->header_fields);
    dictionary_destructor(&request->body);
}

// extract query params from URI
void extract_query_params(struct HTTPRequest *request, char *uri)
{
    int uri_len = strlen(uri);
    int idx = -1;
    for(int i=0; i<uri_len; i++){
        if(uri[i] == '?'){
            idx = i;
            break;
        }
    }


    if(idx == -1){
        return;
    }

    struct Dictionary query_params = dictionary_constructor(compare_string_keys);

    char *copy_uri = malloc(uri_len + 1);
    memset(copy_uri, 0, uri_len + 1);
    strncpy(copy_uri, uri+idx+1, uri_len - idx + 1);

    struct Queue fields = queue_constructor();
    char *field = strtok(copy_uri, "&");
    while (field)
    {
        fields.push(&fields, field, sizeof(char[strlen(field)]));
        field = strtok(NULL, "&");
    }
    // Iterate over the queue to further separate keys from values.
    field = fields.peek(&fields);
    while (field)
    {
        char *key = strtok(field, "=");
        char *value = strtok(NULL, "\0");
        // Remove unnecessary leading white space.
        if (value[0] == ' ')
        {
            value++;
        }
        // Insert the key value pair into the dictionary.
        query_params.insert(&query_params, key, sizeof(char[strlen(key)]), value, sizeof(char[strlen(value)]));
        // Collect the next item in the queue.
        fields.pop(&fields);
        field = fields.peek(&fields);
    }

    request->query_params = query_params;

    queue_destructor(&fields);

    free(copy_uri);
}


void extract_request_line_fields(struct HTTPRequest *request, char *request_line)
{
    // Copy the string literal into a local instance.
    char fields[strlen(request_line)];
    strcpy(fields, request_line);
    // Separate the string on spaces for each section.
    char *method = strtok(fields, " ");
    char *uri = strtok(NULL, " ");
    char *http_version = strtok(NULL, "\0");
    // Insert the results into the request object as a dictionary.
    struct Dictionary request_line_dict = dictionary_constructor(compare_string_keys);
    request_line_dict.insert(&request_line_dict, "method", sizeof("method"), method, sizeof(char[strlen(method)]));
    request_line_dict.insert(&request_line_dict, "uri", sizeof("uri"), uri, sizeof(char[strlen(uri)]));
    // Extracting Query params from uri
    extract_query_params(request, uri);

    request_line_dict.insert(&request_line_dict, "http_version", sizeof("http_version"), http_version, sizeof(char[strlen(http_version)]));
    // Save the dictionary to the request object.
    request->request_line = request_line_dict;
}

// Parses out the header fields.
void extract_header_fields(struct HTTPRequest *request, char *header_fields)
{
    // Copy the string literal into a local instance.
    char fields[strlen(header_fields)];
    strcpy(fields, header_fields);
    // Save each line of the input into a queue.
    struct Queue headers = queue_constructor();
    char *field = strtok(fields, "\n");
    while (field)
    {
        headers.push(&headers, field, sizeof(char[strlen(field)]));
        field = strtok(NULL, "\n");
    }
    // Initialize the request's header_fields dictionary.
    request->header_fields = dictionary_constructor(compare_string_keys);
    // Use the queue to further extract key value pairs.
    char *header = (char *)headers.peek(&headers);
    while (header)
    {
        char *key = strtok(header, ":");
        char *value = strtok(NULL, "\0");
        if (value)
        {
            // Remove leading white spaces.
            if (value[0] == ' ')
            {
                value++;
            }
            // Push the key value pairs into the request's header_fields dictionary.
            request->header_fields.insert(&request->header_fields, key, sizeof(char[strlen(key)]), value, sizeof(char[strlen(value)]));
            // Collect the next field from the queue.
        }
        headers.pop(&headers);
        header = (char *)headers.peek(&headers);
    }
    // Destroy the queue.
    queue_destructor(&headers);
}

// Parses the body according to the content type specified in the header fields.
void extract_body(struct HTTPRequest *request, char *body)
{
    // Check what content type needs to be parsed
    char *content_type = (char *)request->header_fields.search(&request->header_fields, "Content-Type", sizeof("Content-Type"));
    
    if (content_type)
    {
        char *q = strchr(content_type, '\r');
        *q = '\0';
        // Initialize the body_fields dictionary.
        struct Dictionary body_fields = dictionary_constructor(compare_string_keys);
        if (strcmp(content_type, "application/x-www-form-urlencoded") == 0)
        {
            if(*body == '\n'){
                body = body+1;
            }
            // Collect each key value pair as a set and store them in a queue.
            struct Queue fields = queue_constructor();
            char *field = strtok(body, "&");
            while (field)
            {
                fields.push(&fields, field, sizeof(char[strlen(field)]));
                field = strtok(NULL, "&");
            }
            // Iterate over the queue to further separate keys from values.
            field = fields.peek(&fields);
            while (field)
            {
                char *key = strtok(field, "=");
                char *value = strtok(NULL, "\0");
                // Remove unnecessary leading white space.
                if (value[0] == ' ')
                {
                    value++;
                }
                // Insert the key value pair into the dictionary.
                body_fields.insert(&body_fields, key, sizeof(char[strlen(key)]), value, sizeof(char[strlen(value)]));
                // Collect the next item in the queue.
                fields.pop(&fields);
                field = fields.peek(&fields);
            }
            // Destroy the queue.
            queue_destructor(&fields);
        }
        else if (strcmp(content_type, "text/plain") == 0)
        {
            // Save the data as a single key value pair.
            body_fields.insert(&body_fields, "data", sizeof("data"), body, sizeof(char[strlen(body)]));
        }
        else if (strcmp(content_type, "application/json") == 0)
        {
            // JSON parser
            parse_json_object(body, &body_fields);
        }
        else
        {
            // Save the data as a single key value pair.
            body_fields.insert(&body_fields, "data", sizeof("data"), body, sizeof(char[strlen(body)]));
        }
        // Set the request's body dictionary.
        request->body = body_fields;
    }
}
