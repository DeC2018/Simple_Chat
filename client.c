#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
 
int main() {
    // Create TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Configure server address
    struct sockaddr_in address = {
        AF_INET,          // IPv4
        htons(9999),      // Port
        0                 // Connect to localhost
    };
    
    // Connect to server
    connect(sockfd, (struct sockaddr*)&address, sizeof(address));
    
    // Configure poll structure
    struct pollfd fds[2] = {
        {0, POLLIN, 0},    // stdin
        {sockfd, POLLIN, 0} // server socket
    };
    
    // Main communication loop
    for (;;) {
        char buffer[256] = {0};
        
        // Wait for events (50s timeout)
        poll(fds, 2, 50000);
        
        // Handle stdin input
        if (fds[0].revents & POLLIN) {
            read(0, buffer, 255);
            send(sockfd, buffer, 255, 0);
        }
        // Handle server message
        else if (fds[1].revents & POLLIN) {
            if (recv(sockfd, buffer, 255, 0) == 0) {
                // Connection closed
                return 0;
            }
            printf("Received: %s\n", buffer);
        }
    }
    
    return 0;
}
