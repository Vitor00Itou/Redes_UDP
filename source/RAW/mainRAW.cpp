// Trabalho para disciplina de redes

// Alunos:			João Victor Silva Bezerra Nascimento 
// 			        20200007760

//                 André Vitor Oliveira Brito
//                 20210066943

// Professor:      Fernando Menezes Matos

#include <iostream>
#include <string>
#include "ClientRAW.h"
#include <iomanip>

int main(void){
    std::string stringInput;
    int option;
    unsigned char recvBuffer[1024];
    unsigned int numberResponse;
    GeneralClient *client;
    
    try
    {
        client = new ClientRAW("15.228.191.109", 50000);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 0;
    }

    while (true)
    {
        std::cout << "********************************************************************" << std::endl;
        std::cout << "Selecione uma opcão de requisição para o servidor:" << std::endl;
        std::cout << "1. Data e hora atual." << std::endl;
        std::cout << "2. Uma mensagem motivacional para o fim do semestre." << std::endl;
        std::cout << "3. A quantidade de respostas emitidas pelo servidor até o momento." << std::endl;
        std::cout << "4. Sair." << std::endl;

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
        case 1:
            std::cout << "Opção 1 selecionada." << std::endl;

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
        case 2:
            std::cout << "Opção 2 selecionada." << std::endl;

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
        case 3:
            std::cout << "Opção 3 selecionada." << std::endl;

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
        case 4:
            std::cout << "Saindo..." << std::endl;
            delete client;
            return 0;
        default:
            std::cout << "Opção inexistente." << std::endl;
            break;
        }
    }
    
}