#ifndef SERVER_TEMPLATE
#define SERVER_TEMPLATE

#include "DataStructures/Dictionary/Dictionary.h"

#include <sys/socket.h>
#include <netinet/in.h>

struct Server {
    int domain;
    int service;

    int protocol;
    unsigned long interface;

    int port;
    int backlog;

    struct sockaddr_in address;

    int socket;

    struct Dictionary routes;

    void (*launch)(struct Server *server);
};

struct Server server_constructor(int domain, int service, int protocol, unsigned long interface, int port, int backlog);

#endif