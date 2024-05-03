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

ClientUDP::ClientUDP(const char *ip, int port) {
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

bool ClientUDP::receiveMessage(unsigned char* buffer, size_t buffer_size) {
    ssize_t recv_len = recvfrom(sockfd, buffer, buffer_size, 0, nullptr, nullptr);
    if (recv_len < 0) {
        return false;
    }
    buffer[recv_len] = '\0';  // Adiciona um terminador de string
    return true;
}

bool ClientUDP::SendMessageType0(unsigned char* buffer, size_t buffer_size){
    // Tipo 0
    messageBytes[0] = 0x00;

    radomizeLast2bytesMessage();

    if (!sendMessage(messageBytes)) {
        throw std::runtime_error("Erro ao enviar mensagem.");
    }

    if (!receiveMessage(response, sizeof(response))) {
        throw std::runtime_error("Erro ao receber mensagem.");
    }

    if (response[0] != 0x10)
    {
        throw std::runtime_error("Erro na formatação da mensagem recebida.");
    }
    
    if (response[1] != messageBytes[1] || response[2] != messageBytes[2])
    {
        throw std::runtime_error("Identificador da mensagem recebida foi diferente do esperado.");
    }

    int sizeMessage = response[3];

    for (size_t i = 0; i < sizeMessage; i++)
    {
        buffer[i] = response[i+4];
    }

    return true;
}

bool ClientUDP::SendMessageType1(unsigned char* buffer, size_t buffer_size){
    // Tipo 1
    messageBytes[0] = 0x01;

    radomizeLast2bytesMessage();

    if (!sendMessage(messageBytes)) {
        throw std::runtime_error("Erro ao enviar mensagem.");
    }

    if (!receiveMessage(response, sizeof(response))) {
        throw std::runtime_error("Erro ao receber mensagem.");
    }

    if (response[0] != 0x11)
    {
        throw std::runtime_error("Erro na formatação da mensagem recebida.");
    }
    
    if (response[1] != messageBytes[1] || response[2] != messageBytes[2])
    {
        throw std::runtime_error("Identificador da mensagem recebida foi diferente do esperado.");
    }

    int sizeMessage = response[3];

    for (size_t i = 0; i < sizeMessage; i++)
    {
        buffer[i] = response[i+4];
    }

    return true;
}

bool ClientUDP::SendMessageType2(unsigned int* resp){
    unsigned char number4Bytes[4];
    unsigned int numberResponse;

    // Tipo 2
    messageBytes[0] = 0x02;

    radomizeLast2bytesMessage();

    if (!sendMessage(messageBytes)) {
        throw std::runtime_error("Erro ao enviar mensagem.");
    }

    if (!receiveMessage(response, sizeof(response))) {
        throw std::runtime_error("Erro ao receber mensagem.");
    }

    if (response[0] != 0x12 || response[3] != 0x04)
    {
        throw std::runtime_error("Erro na formatação da mensagem recebida.");
    }
   
    if (response[1] != messageBytes[1] || response[2] != messageBytes[2])
    {
        throw std::runtime_error("Identificador da mensagem recebida foi diferente do esperado.");
    }

    for (size_t i = 0; i < 4; i++)
    {
        number4Bytes[i] = response[i+4];
    }
    
    numberResponse = charsToInt(number4Bytes);

    *resp = numberResponse;

    return true;
    

}

void ClientUDP::radomizeLast2bytesMessage(){
    // Seed para gerar números aleatórios
    std::srand(std::time(nullptr));

    // Gerar um número aleatório entre 1 e 65535
    int randomNumber = (std::rand() % 65535) + 1;

    // Extrair os bytes mais e menos significativos
    unsigned char byte1 = static_cast<unsigned char>((randomNumber >> 8) & 0xFF); // Byte mais significativo
    unsigned char byte2 = static_cast<unsigned char>(randomNumber & 0xFF); // Byte menos significativo

    // Exibir os bytes em hexadecimal
    messageBytes[1] = byte1;
    messageBytes[2] = byte2;

}

unsigned int ClientUDP::charsToInt(const unsigned char* chars) {
    return (static_cast<unsigned int>(static_cast<unsigned char>(chars[0])) << 24) |
           (static_cast<unsigned int>(static_cast<unsigned char>(chars[1])) << 16) |
           (static_cast<unsigned int>(static_cast<unsigned char>(chars[2])) << 8) |
           static_cast<unsigned int>(static_cast<unsigned char>(chars[3]));
}