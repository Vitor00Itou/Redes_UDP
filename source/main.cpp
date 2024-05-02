#include <iostream>
#include <string>
#include "ClientUDP.h"
#include <iomanip>


int main(void){
    std::string stringInput;
    int option;
    unsigned char response[1024];
    unsigned int numberResponse;


    ClientUDP *client = new ClientUDP("15.228.191.109", 50000);

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
                client->SendMessageType0(response, sizeof(response));
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                break;
            }
            
            std::cout << "Resposta do servidor: " << response << std::endl;

            break;
        case 2:
            std::cout << "Opção 2 selecionada." << std::endl;

            try
            {
                client->SendMessageType1(response, sizeof(response));
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                break;
            }
            
            std::cout << "Resposta do servidor: " << response << std::endl;

            break;
        case 3:
            std::cout << "Opção 3 selecionada." << std::endl;

            try
            {
                client->SendMessageType2(&numberResponse);
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
            return 0;
        default:
            std::cout << "Opção inexistente." << std::endl;
            break;
        }
    }
    
}