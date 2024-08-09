#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>

#define PORT 8080
#define BUFFER_SIZE 1024

std::map<std::string, int> reservations;

std::string generateConfirmation(int numGuests, std::string date) {
    if (reservations.find(date) != reservations.end()) {
        if (reservations[date] + numGuests <= 10) {
            reservations[date] += numGuests;
            return "Reservation confirmed for " + date + " with " + std::to_string(numGuests) + " guests.";
        } else {
            return "Sorry, we are fully booked for " + date + ".";
        }
    } else {
        reservations[date] = numGuests;
        return "Reservation confirmed for " + date + " with " + std::to_string(numGuests) + " guests.";
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    std::string message = "Welcome to online reservation system! Please enter your reservation details:\n";

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Set up address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return 1;
    }

    std::cout << "Server connected. Waiting for reservations..." << std::endl;

    while (true) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        std::cout << "New reservation request." << std::endl;

        // Send welcome message to client
        send(new_socket, message.c_str(), message.length(), 0);

        // Receive reservation request from client
        int numGuests;
        read(new_socket, &numGuests, sizeof(int));

        char dateBuffer[BUFFER_SIZE] = {0};
        read(new_socket, dateBuffer, BUFFER_SIZE);
        std::string date = dateBuffer;

        // Generate reservation confirmation
        std::string confirmation = generateConfirmation(numGuests, date);
        
        //Print confirmation to console
        std::cout << "Reservation confirmation: " << confirmation << std::endl;

        // Send reservation confirmation
        send(new_socket, confirmation.c_str(), confirmation.length(), 0);
        
        //close socket
        close(new_socket);
        }
        return 0;
      }  

