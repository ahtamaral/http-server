#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8090

void handle_client( int client_socket )
{
    char buffer[30000] = {0};
    read(client_socket, buffer, 30000);

    std::cout << "Received request:\n" << buffer << std::endl;

    while(true){}

    // Cria uma resposta HTTP básica
    std::string response =
        "HTTP/1.1 200 OK\n"
        "Content-Type: text/html\n"
        "Content-Length: 88\n\n"
        "<html>\n"
        "<head><title>Simple C++ HTTP Server</title></head>\n"
        "<body><h1>Hello from C++ Server!</h1></body>\n"
        "</html>";

    // Envia a resposta ao cliente
    send(client_socket, response.c_str(), response.length(), 0);
    std::cout << "Response sent\n";

    // Fecha a conexão com o cliente
    close(client_socket);
}

int main()
{
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Cria o socket do servidor
    if ( ( server_fd = socket( AF_INET, SOCK_STREAM, 0 ) ) == 0 )
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Define as propriedades do endereço do servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Associa o socket à porta 8080
    if ( bind( server_fd, (struct sockaddr*) &address, sizeof(address) ) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Inicia o servidor em modo de escuta
    if ( listen( server_fd, 10 ) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "HTTP server listening on port " << PORT << std::endl;

    // Loop para aceitar e processar conexões
    while (true)
    {
        if ( (client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen) ) < 0)
        {
            perror("Accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // Processa a requisição HTTP do cliente
        handle_client(client_socket);
    }

    return 0;
}
