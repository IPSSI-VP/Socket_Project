/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Programme principal
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

#define PORT 4242 // le port du serveur

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

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sa.sin_port = htons(PORT);

    if (bind(socket_fd, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(socket_fd);
        WSACleanup();
        return 1;
    }

    printf("Server listening on UDP port %d...\n", PORT);

    char buffer[BUFSIZ];
    struct sockaddr_in client_addr;
    int addr_size = sizeof(client_addr);

    while (1) {
        int bytes_read = recvfrom(socket_fd, buffer, BUFSIZ, 0,
                                (struct sockaddr*)&client_addr, &addr_size);
                                
        if (bytes_read == SOCKET_ERROR) {
            printf("recvfrom failed: %d\n", WSAGetLastError());
            continue;
        }

        buffer[bytes_read] = '\0';
        printf("Message received: \"%s\"\n", buffer);

        const char* msg = "Got your message.";
        int msg_len = strlen(msg);
        
        if (sendto(socket_fd, msg, msg_len, 0,
                  (struct sockaddr*)&client_addr, addr_size) == SOCKET_ERROR) {
            printf("sendto failed: %d\n", WSAGetLastError());
        } else {
            printf("Response sent to client\n");
        }
    }

    closesocket(socket_fd);
    WSACleanup();
    return 0;
}
