#include "ClientRAW.h"

ClientRAW::ClientRAW()
{
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0) {
        std::cerr << "Erro ao criar o socket." << std::endl;
        return;
    }
    socklen_t addr_len = sizeof(this->local_addr);
    if (getsockname(sockfd, (struct sockaddr *)& this->local_addr, &addr_len) != 0) {
        std::cerr << "Erro ao obter a porta local." << std::endl;
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2222);  // Porta do servidor
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);  // Endereço localhost
}

ClientRAW::ClientRAW(const char* ip, int port)
{
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0) {
        std::cerr << "Erro ao criar o socket." << std::endl;
        return;
    }
    socklen_t addr_len = sizeof(this->local_addr);
    if (getsockname(sockfd, (struct sockaddr *)& this->local_addr, &addr_len) != 0) {
        std::cerr << "Erro ao obter a porta local." << std::endl;
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);  // Porta do servidor
    inet_pton(AF_INET, ip, &server_addr.sin_addr);  // Endereço escolhido
}

ClientRAW::~ClientRAW()
{
	close(sockfd);
}

bool ClientRAW::sendMessage(unsigned char* rawMessage) {
    // Pacote UDP a ser construido com 8 bytes de cabeçalho e 3 de payload
    unsigned char udpPkt[APP_PROTO_REQ_SIZE + UDP_HEADER_SIZE] = {0}; // Inicializando o array de bytes com 0

    printf("Orig port: %d\n", local_addr.sin_port);
    printf("Orig MSBs: %x\n", local_addr.sin_port);
    printf("Orig LSBs: %x\n", local_addr.sin_port);
    // Colocando a porta de origem
    udpPkt[0] = (local_addr.sin_port >> 8) & 0xFF;
    udpPkt[1] = local_addr.sin_port & 0xFF;
    
    printf("Dest port: %d\n", server_addr.sin_port);
    printf("Dest MSBs: %x\n", server_addr.sin_port);
    printf("Dest LSBs: %x\n", server_addr.sin_port);
    // Colocando a porta de destino
    udpPkt[2] = (server_addr.sin_port >> 8) & 0xFF;
    udpPkt[3] = server_addr.sin_port & 0xFF;

    printf("Seg size MSBs: %x\n", (SEGMENT_SIZE >> 8) & 0xFF);
    printf("Seg size LSBs: %x\n", SEGMENT_SIZE & 0xFF);
    // Colocando o tamanho do segmento
    udpPkt[4] = (SEGMENT_SIZE >> 8) & 0xFF;
    udpPkt[5] = SEGMENT_SIZE & 0xFF;

    // Colocando o payload
    for (int i = 8; i < APP_PROTO_REQ_SIZE + UDP_HEADER_SIZE; i++) {
        udpPkt[i] = rawMessage[i];
    }

    // Colocando o checksum
    unsigned short checksum = getChecksum(udpPkt, APP_PROTO_REQ_SIZE+UDP_HEADER_SIZE);
    udpPkt[6] = (checksum >> 8) & 0xFF;
    udpPkt[7] = checksum & 0xFF;

    ssize_t send_len = sendto(sockfd, udpPkt, sizeof(udpPkt), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    return send_len >= 0;
}

bool ClientRAW::receiveMessage(unsigned char* buffer, size_t buffer_size) {
    ssize_t recv_len = recvfrom(sockfd, buffer, buffer_size, 0, nullptr, nullptr);
    if (recv_len < 0) {
        return false;
    }
    buffer[recv_len] = '\0';  // Adiciona um terminador de string
    return true;
}

unsigned short ClientRAW::getChecksum(unsigned char* segment, int segSize) {
    unsigned char* auxPacket = segment;
    if (segSize % 2 == 0) {
        segSize++;
        auxPacket = (unsigned char*) malloc(segSize);
    }
    unsigned int chksum = 0;
    // Pseudoheader byte a byte: ipOrig, ipDest, protocolo de transporte, comprimento do segmento UDP
    unsigned short pseudoIPHeader[] = {0xC0, 0xA8, 0x01, 0x69, 0x0F, 0xE4, 0xBF, 0x6D, 0x00, 0x11, 0x00, 0x0B};

    // Somando o pseudoheader
    for (int i = 0; i < 8; i++) {
        chksum += (unsigned short) ((pseudoIPHeader[i] << 8) | (auxPacket[i+1]));
    }

    // Somando o cabeçalho UDP e payload (segmento UDP)
    for (int i = 0; i < segSize; i += 2) {
        chksum += (unsigned short) ((auxPacket[i] << 8) | (auxPacket[i+1]));
    }
    // Fazendo o wrapparound
    chksum = ((chksum >> 16) & 0xFFFF) + (chksum & 0xFFFF);

    std::cout << "Checksum calculado: " << ~chksum << std::endl;
    return ~chksum; // Fazendo o complemento de 1
}