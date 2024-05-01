#include "ClientUDP.h"

ClientUDP::ClientUDP() {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Erro ao criar o socket." << std::endl;
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2222);  // Porta do servidor
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);  // Endereço localhost
}

ClientUDP::ClientUDP(char *ip, int port) {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Erro ao criar o socket." << std::endl;
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);  // Porta do servidor
    inet_pton(AF_INET, ip, &server_addr.sin_addr);  // Endereço localhost
}

ClientUDP::~ClientUDP()
{
    close(sockfd);
}


bool ClientUDP::sendMessage(unsigned char* message) {
    ssize_t send_len = sendto(sockfd, message, sizeof(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    return send_len >= 0;
}

bool ClientUDP::receiveMessage(char* buffer, size_t buffer_size) {
    ssize_t recv_len = recvfrom(sockfd, buffer, buffer_size, 0, nullptr, nullptr);
    if (recv_len < 0) {
        return false;
    }
    buffer[recv_len] = '\0';  // Adiciona um terminador de string
    return true;
}
