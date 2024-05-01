#ifndef CLIENTUDP_H
#define CLIENTUDP_H

#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

class ClientUDP
{
public:
    ClientUDP();
    ClientUDP(char *ip, int port);
    ~ClientUDP();

    bool sendMessage(unsigned char *menssage);
    bool receiveMessage(char* buffer, size_t buffer_size);

private:
    int sockfd;
    struct sockaddr_in server_addr;

};

#endif