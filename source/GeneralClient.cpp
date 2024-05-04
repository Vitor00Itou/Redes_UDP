#include "GeneralClient.h"

GeneralClient::GeneralClient()
{

}

GeneralClient::~GeneralClient()
{

}

void GeneralClient::randomizeLast2bytesMessage(){
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

unsigned int GeneralClient::charsToInt(const unsigned char* chars) {
    return (static_cast<unsigned int>(static_cast<unsigned char>(chars[0])) << 24) |
           (static_cast<unsigned int>(static_cast<unsigned char>(chars[1])) << 16) |
           (static_cast<unsigned int>(static_cast<unsigned char>(chars[2])) << 8) |
           static_cast<unsigned int>(static_cast<unsigned char>(chars[3]));
}

bool GeneralClient::handleType0Message(unsigned char* buffer, size_t buffer_size){
    // Tipo 0
    messageBytes[0] = 0x00;

    randomizeLast2bytesMessage();

    if (!sendMessage(messageBytes)) {
        throw std::runtime_error("Erro ao enviar mensagem.");
    }

    if (!receiveMessage(recvBuffer, sizeof(recvBuffer))) {
        throw std::runtime_error("Erro ao receber mensagem.");
    }

    if (recvBuffer[0] != 0x10)
    {
        throw std::runtime_error("Erro na formatação da mensagem recebida.");
    }
    
    if (recvBuffer[1] != messageBytes[1] || recvBuffer[2] != messageBytes[2])
    {
        throw std::runtime_error("Identificador da mensagem recebida foi diferente do esperado.");
    }

    int messageSize = recvBuffer[3];

    for (size_t i = 0; i < messageSize; i++)
    {
        buffer[i] = recvBuffer[i+4];
    }

    return true;
}

bool GeneralClient::handleType1Message(unsigned char* buffer, size_t buffer_size){
    // Tipo 1
    messageBytes[0] = 0x01;

    randomizeLast2bytesMessage();

    if (!sendMessage(messageBytes)) {
        throw std::runtime_error("Erro ao enviar mensagem.");
    }

    if (!receiveMessage(recvBuffer, sizeof(recvBuffer))) {
        throw std::runtime_error("Erro ao receber mensagem.");
    }

    if (recvBuffer[0] != 0x11)
    {
        throw std::runtime_error("Erro na formatação da mensagem recebida.");
    }
    
    if (recvBuffer[1] != messageBytes[1] || recvBuffer[2] != messageBytes[2])
    {
        throw std::runtime_error("Identificador da mensagem recebida foi diferente do esperado.");
    }

    int messageSize = recvBuffer[3];

    for (size_t i = 0; i < messageSize; i++)
    {
        buffer[i] = recvBuffer[i+4];
    }

    return true;
}

bool GeneralClient::handleType2Message(unsigned int* resp){
    unsigned char number4Bytes[4];
    unsigned int numberResponse;

    // Tipo 2
    messageBytes[0] = 0x02;

    randomizeLast2bytesMessage();

    if (!sendMessage(messageBytes)) {
        throw std::runtime_error("Erro ao enviar mensagem.");
    }

    if (!receiveMessage(recvBuffer, sizeof(recvBuffer))) {
        throw std::runtime_error("Erro ao receber mensagem.");
    }

    if (recvBuffer[0] != 0x12 || recvBuffer[3] != 0x04)
    {
        throw std::runtime_error("Erro na formatação da mensagem recebida.");
    }
   
    if (recvBuffer[1] != messageBytes[1] || recvBuffer[2] != messageBytes[2])
    {
        throw std::runtime_error("Identificador da mensagem recebida foi diferente do esperado.");
    }

    for (size_t i = 0; i < 4; i++)
    {
        number4Bytes[i] = recvBuffer[i+4];
    }
    
    numberResponse = charsToInt(number4Bytes);

    *resp = numberResponse;

    return true;
    

}