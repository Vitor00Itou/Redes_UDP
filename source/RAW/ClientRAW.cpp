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

bool ClientRAW::sendMessage(u_char* rawMessage) {
    // Pacote UDP a ser construido com 8 bytes de cabeçalho e 3 de payload
    u_char udpPkt[SEGMENT_SIZE] = {0}; // Inicializando o array de bytes com 0

    // Colocando a porta de origem
    uint16_t aux = ntohs(local_addr.sin_port); // Passando para big-endian
    // Porta 22222
    udpPkt[0] = 0x56;
    udpPkt[1] = 0xCE;

    // Colocando a porta de destino
    aux = ntohs(server_addr.sin_port); // Passando para big-endian
    udpPkt[2] = static_cast<u_char>(aux >> 8);
    udpPkt[3] = static_cast<u_char>(aux);

    // Colocando o tamanho do segmento
    udpPkt[4] = static_cast<u_char>(SEGMENT_SIZE >> 8); // MSBs de 0x000B
    udpPkt[5] = static_cast<u_char>(SEGMENT_SIZE); // LSBs de 0x000B

    // Colocando o payload
    for (int i = 0; i < 3; i++) {
        udpPkt[i+UDP_HEADER_SIZE] = rawMessage[i];
    }
    // udpPkt[8] = 0x0;
    // udpPkt[9] = 0x16;
    // udpPkt[10] = 0xa1;

    // Colocando o checksum
    u_short chksum = computeChecksum(udpPkt, SEGMENT_SIZE);
    udpPkt[6] = static_cast<u_char>(chksum >> 8);
    udpPkt[7] = static_cast<u_char>(chksum);

    // puts("Segment:");
    // for (int i = 0; i < SEGMENT_SIZE; i++) {
    //     printf("%x ", udpPkt[i]);
    // }
    // puts("");

    // Enviando a requisição de fato
    ssize_t send_len = sendto(sockfd, udpPkt, sizeof(udpPkt), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    return send_len >= 0;
}

bool ClientRAW::receiveMessage(u_char* buffer, size_t buffer_size) {
    const int headerSize = UDP_HEADER_SIZE+IPV4_HEADER_SIZE;
    u_char auxBuffer[buffer_size+headerSize];

    ssize_t recv_len = recvfrom(sockfd, auxBuffer, buffer_size+headerSize, 0, nullptr, nullptr);
    if (recv_len < 0) {
        return false;
    }
    puts("Recv buffer:");
    for (int i = 0; i < recv_len; i++) {
        if (i < headerSize)
            printf("%x-", auxBuffer[i]);
        else
            printf("%x-", auxBuffer[i]);
    }
    puts("");
    for (int i = 0; i < recv_len; i++)
        buffer[i] = auxBuffer[i+headerSize];
    buffer[recv_len] = '\0';  // Adiciona um terminador de string
    puts("Recv buffer em hex:");
    for (int i = 0; i < buffer[3]+4; i++) {
        printf("%x-", buffer[i]);
    }
    puts("\nRecv buffer em char:");
    for (int i = 0; i < buffer[3]+4; i++) {
        printf("%c", buffer[i]);
    }
    puts("");
    return true;
}

u_short ClientRAW::computeChecksum(u_char* segment, int segSize) {
    u_char* auxSegment = segment;
    bool padded = false;
    
    // Fazendo o padding do último byte, caso o tamanho seja ímpar
    if (segSize % 2 == 0) {
        padded = true;
        auxSegment = static_cast<u_char*>(calloc(segSize, sizeof(u_char)));
        for (int i = 0; i < segSize; i++)
            auxSegment[i] = segment[i]; // Copiando os valores pra o segmento de padding
        segSize++;
    }

    u_int chksum = 0;

    // Somando o pseudoheader
    u_int aux = getLocalIPv4(); // Little-endian
    chksum += static_cast<u_short>(aux >> 16); // LSBytes do IP local
    chksum += static_cast<u_short>(aux); // MSBytes do IP local

    aux = ntohl(server_addr.sin_addr.s_addr); // Little-endian
    chksum += static_cast<u_short>(aux >> 16); // LSBytes do IP de destino
    chksum += static_cast<u_short>(aux); // MSBytes do IP de destino

    chksum += 0x0011; // Número do protocolo UDP
    chksum += 0x000B; // Tamanho do payload

    // Somando o cabeçalho UDP e payload (segmento UDP)
    for (int i = 0; i < segSize; i += 2) {
        chksum += static_cast<u_short>((auxSegment[i] << 8) | static_cast<u_short>(auxSegment[i+1]));
    }
    
    // Fazendo o wrapparound
    chksum = (static_cast<u_short>(chksum >> 16)) + static_cast<u_short>(chksum);

    if (padded)
        free(auxSegment);
    return ~chksum; // Fazendo o complemento de 1
}

/**
 *  Retorna o endereço IPv4 como um inteiro de 32 bits em formato little-endian.
 */
u_int ClientRAW::getLocalIPv4() {
    // Pegando as interfaces de rede locais
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        std::cerr << "Erro ao obter o endereço IPv4 local.\n";
        return -1;
    }

    // Iterando sobre as interfaces de rede
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        
        // Pegando a primeira interface com conexão de internet e sem ser o loopback
        if (ifa->ifa_addr != nullptr && ifa->ifa_addr->sa_family == AF_INET && strcmp(ifa->ifa_name, "lo") != 0) {
            // Pegando o IPv4 em little-endian
            struct sockaddr_in *addr = reinterpret_cast<struct sockaddr_in*>(ifa->ifa_addr);
            u_int ipv4 = ntohl(addr->sin_addr.s_addr);
            freeifaddrs(ifaddr);
            return ipv4;
        }
    }
    return 0;
}