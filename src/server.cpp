#include <iostream>
#include <string>
#include <cstring>     // For memset
#include <unistd.h>    // For close()
#include <arpa/inet.h> // For sockaddr_in and inet functions
#include <sys/socket.h>

int main() {

    int s_fd = socket(AF_INET, SOCK_STREAM, 0);  // set fd of socket to use IPv4 and TCP
    if (s_fd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Get address looking right
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(8080);

    // Bind

    if (bind(s_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(s_fd); // Must close if bind fails
        return 1;
    }

    // Listen
    if (listen(s_fd, 3) < 0) {
        perror("Listen failed");
        close(s_fd);
        return 1;
    }

    std::cout << "Server manual mode: Listening on port 8080...\n";

    // Accept conn
    sockaddr_in client_addr{};
    socklen_t addr_len = sizeof(client_addr);
    int c_fd = accept(s_fd, (struct sockaddr*)&client_addr, &addr_len);
    
    if (c_fd < 0) {
        perror("Accept failed");
        close(s_fd);
        return 1;
    }

    // Comm loop
    char buffer[1024];
    while (true) {
        ssize_t bytes_read = recv(c_fd, buffer, sizeof(buffer), 0);
        if (bytes_read <= 0) break; // if client disconnects

        send(c_fd, buffer, bytes_read, 0);
    }

    // Manual clean up of fd - 
    // TODO: change to RAII
    std::cout << "Closing connections.\n";
    close(c_fd);
    close(s_fd);

    return 0;
}