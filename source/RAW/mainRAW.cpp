#include <iostream>
#include <string>
#include "ClientRAW.h"
#include <iomanip>

int main(void){
    std::string stringInput;
    int option;
    unsigned char recvBuffer[1024];
    unsigned int numberResponse;

    GeneralClient *client = new ClientRAW("15.228.191.109", 50000);

    while (true)
    {
        std::cout << "********************************************************************" << std::endl;
        std::cout << "Selecione uma opcão de requisição para o servidor:" << std::endl;
        std::cout << "0. Data e hora atual." << std::endl;
        std::cout << "1. Uma mensagem motivacional para o fim do semestre." << std::endl;
        std::cout << "2. A quantidade de respostas emitidas pelo servidor até o momento." << std::endl;
        std::cout << "3. Sair." << std::endl;

        std::cin >> stringInput;

        // Tratamento de erro para caso o usuário digite algo inesperado
        try
        {
            option = std::stoi(stringInput);
        }
        catch(const std::exception& e)
        {
            std::cout << "Opção inválida." << std::endl;
            continue;
        }

        switch (option)
        {
        case 0:
            std::cout << "Opção 0 selecionada." << std::endl;

            try
            {
                client->handleType0Message(recvBuffer, sizeof(recvBuffer));
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                break;
            }
            
            std::cout << "Resposta do servidor: " << recvBuffer << std::endl;

            break;
        case 1:
            std::cout << "Opção 1 selecionada." << std::endl;

            try
            {
                client->handleType1Message(recvBuffer, sizeof(recvBuffer));
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                break;
            }
            
            std::cout << "Resposta do servidor: " << recvBuffer << std::endl;

            break;
        case 2:
            std::cout << "Opção 2 selecionada." << std::endl;

            try
            {
                client->handleType2Message(&numberResponse);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                break;
            }
            
            std::cout << "Resposta do servidor: " << std::dec << numberResponse << std::endl;

            break;
        case 3:
            std::cout << "Saindo..." << std::endl;
            return 0;
        default:
            std::cout << "Opção inexistente." << std::endl;
            break;
        }
    }
    
}