#ifndef CLIENTUDP_H
#define CLIENTUDP_H

#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "../GeneralClient.h"

class ClientUDP : GeneralClient
{
public:
    ClientUDP();
    ClientUDP(const char *ip, int port);
    ~ClientUDP();

    bool sendMessage(unsigned char *message);
    bool receiveMessage(unsigned char* buffer, size_t buffer_size);

    bool SendMessageType0(unsigned char* buffer, size_t buffer_size);
    bool SendMessageType1(unsigned char* buffer, size_t buffer_size);
    bool SendMessageType2(unsigned int* resp);
};

#endif