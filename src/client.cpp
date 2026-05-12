#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    // 1. Create socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Socket creation error");
        return 1;
    }

    // 2. Setup Server Address
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Convert IPv4 address from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        close(sock_fd);
        return 1;
    }

    // 3. Connect
    if (connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        close(sock_fd);
        return 1;
    }

    // 4. Send & Receive
    std::string userInput{};
    char buffer[1024] = {0};
    while(true) {

        getline(std::cin, userInput);
        if (userInput == "quit") break;

        send(sock_fd, userInput.c_str(), userInput.length(), 0);


        ssize_t valread = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
        if (valread > 0) {
            std::string_view part(buffer, valread);
            std::cout << "Received: " << part << std::endl;
        }
    }

    // std::string msg = "Hello from the manual client!";
    // send(sock_fd, msg.c_str(), msg.length(), 0);

    // 5. Manual Cleanup
    close(sock_fd);
    return 0;
}