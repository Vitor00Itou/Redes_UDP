#ifndef CLIENTUDP_H
#define CLIENTUDP_H

#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <ctime>


class ClientUDP
{
public:
    ClientUDP();
    ClientUDP(const char *ip, int port);
    ~ClientUDP();

    bool sendMessage(unsigned char *menssage);
    bool receiveMessage(unsigned char* buffer, size_t buffer_size);

    bool SendMessageType0(unsigned char* buffer, size_t buffer_size);
    bool SendMessageType1(unsigned char* buffer, size_t buffer_size);
    bool SendMessageType2(unsigned int* resp);

private:
    void radomizeLast2bytesMessage();
    unsigned int charsToInt(const unsigned char* chars);

    int sockfd;
    struct sockaddr_in server_addr;
    unsigned char response[1024];
    unsigned char messageBytes[3];
};

#endif