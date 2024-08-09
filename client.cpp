#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Set up address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return 1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // Receive welcome message from server
    char buffer[BUFFER_SIZE] = {0};
    read(sock, buffer, BUFFER_SIZE);
    std::cout << buffer << std::endl;

    // Send reservation request to server
    int numGuests;
    std::cout << "Enter number of guests: ";
    std::cin >> numGuests;
    send(sock, &numGuests, sizeof(int), 0);

    std::string date;
    std::cout << "Enter date (YYYY-MM-DD): ";
    std::cin >> date;
    send(sock, date.c_str(), date.length(), 0);

    // Receive reservation confirmation from server
    read(sock, buffer, BUFFER_SIZE);
    std::cout << "Reservation confirmation: " << buffer << std::endl;

    // Close socket
    close(sock);
    return 0;
}

