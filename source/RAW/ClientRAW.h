#ifndef CLIENTRAW_H
#define CLIENTRAW_H

#pragma once

#include "../GeneralClient.h"

#define SEGMENT_SIZE 0x000B // Tamanho em 2 bytes do segmento
#define APP_PROTO_REQ_SIZE 3 // Tamanho do payload de uma requisição nessa aplicação
#define UDP_HEADER_SIZE 8 // Tamanho de um cabeçalho UDP

class ClientRAW : public GeneralClient
{
public:
    ClientRAW();
    ClientRAW(const char* ip, int port);
    ~ClientRAW();

    bool sendMessage(unsigned char *message);
    bool receiveMessage(unsigned char* buffer, size_t buffer_size);
    
private:
    unsigned short getChecksum(unsigned char* packet, int pktSize);

    sockaddr_in local_addr;
};

#endif