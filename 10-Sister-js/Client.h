#ifndef CLIENT
#define CLIENT

#include <netinet/in.h>

struct Client {
    int domain;
    int service;
    int protocol;
    int port;
    unsigned long interface;

    int socket;

    void (*request)(struct Client *client, char *server_ip, char* request);
};

#endif