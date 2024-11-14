#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define SERVER_PORT 8090

int main() {

    // Etapa 1: Cria um socket para comunicação em rede
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error opening socket!" << std::endl;
        return 1;
    }

    // Etapa 2: Define as propiedades do endereço de destino
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost


    // Etapa 3: Tenta estabelecer conexão com servidor
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error connecting to server!" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Sending request...\n";
    
    // Etapa 4: Monta requisição e envia ao servidor
    std::string request = "GET / HTTP/1.1\r\n";
    request += "Host: localhost:8090\r\n";
    request += "Connection: keep-alive\r\n";  // Keep the connection alive
    request += "\r\n";  // End of headers

    ssize_t bytes_sent = send(sockfd, request.c_str(), request.length(), 0);
    if (bytes_sent < 0) {
        std::cerr << "Error sending request!" << std::endl;
        close(sockfd);
        return 1;
    }

    // Etapa 5: Recebe resposta do servidor
    char buffer[4096];
    ssize_t bytes_received;
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        std::cout << buffer << '\n';
    }

    if (bytes_received < 0) {
        std::cerr << "Error receiving response!" << std::endl;
        close(sockfd);
        return 1;
    }

    // Etapa 6: Fecha o socket
    close(sockfd);
    
    return 0;
}