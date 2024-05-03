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