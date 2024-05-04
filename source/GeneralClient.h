#ifndef GENERALCLIENT_H
#define GENERALCLIENT_H

#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <ctime>

class GeneralClient
{
public:
    GeneralClient();
    ~GeneralClient();

    // A serem implementadas pelo cliente de um protocolo específico
    virtual bool sendMessage(unsigned char *menssage) = 0;
    virtual bool receiveMessage(unsigned char* buffer, size_t buffer_size) = 0;

    bool handleType0Message(unsigned char* buffer, size_t buffer_size);
    bool handleType1Message(unsigned char* buffer, size_t buffer_size);
    bool handleType2Message(unsigned int* resp);

protected:
    void randomizeLast2bytesMessage();
    unsigned int charsToInt(const unsigned char* chars);

    int sockfd;
    struct sockaddr_in server_addr;
    unsigned char recvBuffer[1024];
    unsigned char messageBytes[3];
};

#endif