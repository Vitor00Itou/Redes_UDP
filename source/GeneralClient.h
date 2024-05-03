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

    virtual bool sendMessage(unsigned char *menssage) = 0;
    virtual bool receiveMessage(unsigned char* buffer, size_t buffer_size) = 0;

    virtual bool SendMessageType0(unsigned char* buffer, size_t buffer_size) = 0;
    virtual bool SendMessageType1(unsigned char* buffer, size_t buffer_size) = 0;
    virtual bool SendMessageType2(unsigned int* resp) = 0;

protected:
    void randomizeLast2bytesMessage();
    unsigned int charsToInt(const unsigned char* chars);

    int sockfd;
    struct sockaddr_in server_addr;
    unsigned char response[1024];
    unsigned char messageBytes[3];
};

#endif