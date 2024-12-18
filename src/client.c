/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  Client UDP
 *
 *        Version:  1.0
 *        Created:  18/12/2024
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valentin Pizzio
 *   Organization:  
 *
 * =====================================================================================
 */

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 4242
#define SERVER_IP "127.0.0.1"

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    SOCKET socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd == INVALID_SOCKET) {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    char buffer[BUFSIZ];
    printf("Enter message to send (or 'quit' to exit): ");
    
    while (fgets(buffer, BUFSIZ, stdin) != NULL) {
        // Enlever le retour à la ligne de fgets
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "quit") == 0) {
            break;
        }

        if (sendto(socket_fd, buffer, strlen(buffer), 0,
                  (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
            printf("sendto failed: %d\n", WSAGetLastError());
            continue;
        }

        printf("Message sent, waiting for response...\n");

        char recv_buffer[BUFSIZ];
        int server_addr_size = sizeof(server_addr);
        
        int bytes_received = recvfrom(socket_fd, recv_buffer, BUFSIZ, 0,
                                    (struct sockaddr*)&server_addr, &server_addr_size);
                                    
        if (bytes_received == SOCKET_ERROR) {
            printf("recvfrom failed: %d\n", WSAGetLastError());
        } else {
            recv_buffer[bytes_received] = '\0';
            printf("Server response: %s\n", recv_buffer);
        }

        printf("\nEnter message to send (or 'quit' to exit): ");
    }

    closesocket(socket_fd);
    WSACleanup();
    return 0;
}
