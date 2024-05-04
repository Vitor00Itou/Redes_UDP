#ifndef CLIENTRAW_H
#define CLIENTRAW_H

#pragma once

#include "../GeneralClient.h"
#include <ifaddrs.h>

#define APP_REQ_SIZE 3 // Tamanho do payload de uma requisição nessa aplicação
#define UDP_HEADER_SIZE 8 // Tamanho de um cabeçalho UDP
#define SEGMENT_SIZE (APP_REQ_SIZE+UDP_HEADER_SIZE) // Tamanho em 2 bytes do segmento
#define IPV4_HEADER_SIZE 20 // Tamanho de um cabeçalho IPv4

class ClientRAW : public GeneralClient
{
public:
    ClientRAW();
    ClientRAW(const char* ip, int port);
    ~ClientRAW();

    bool sendMessage(unsigned char *message);
    bool receiveMessage(unsigned char* buffer, size_t buffer_size);
    
private:
    u_short computeChecksum(unsigned char* packet, int pktSize);
    u_int getLocalIPv4();

    sockaddr_in local_addr;
};

#endif