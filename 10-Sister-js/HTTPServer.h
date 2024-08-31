#ifndef HTTP_SERVER
#define HTTP_SERVER

#include "server.h"
#include "HTTPRequest.h"

struct HTTPServer {
    struct Server server;
    struct Dictionary routes;

    void (*register_routes)(struct HTTPServer *server, char * (*route_function)(struct HTTPServer *server, struct HTTPRequest *request), char *uri, int num_methods, ...);
    void (*launch)(struct HTTPServer *server);
};

enum HTTPMethods {
    NONE,
    GET,
    POST,
    PUT,
    HEAD,
    PATCH,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
};

struct HTTPServer http_server_constructor(int port);

void launch(struct HTTPServer *server);

char *render_template(char *filepath, struct Dictionary *dictionary);

#endif