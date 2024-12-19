#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 4242  // le port du serveur
#define BACKLOG 10 // nombre max de demandes de connexion

int main()
{
    struct sockaddr_in sa;
    int socket_fd;
    int client_fd;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    char buffer[BUFSIZ];
    int bytes_read;

    // Préparer l'adresse et le port pour la socket du serveur
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;                     // IPv4
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    sa.sin_port = htons(PORT);

    // Créer le socket
    socket_fd = socket(sa.sin_family, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("socket");
        return 1;
    }

    // Associer le socket à l'adresse et au port
    if (bind(socket_fd, (struct sockaddr *)&sa, sizeof sa) != 0)
    {
        perror("bind");
        close(socket_fd);
        return 2;
    }

    // Écouter les connexions entrantes
    if (listen(socket_fd, BACKLOG) != 0)
    {
        perror("listen");
        close(socket_fd);
        return 3;
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1)
    {
        // Accepter une connexion entrante
        addr_size = sizeof client_addr;
        client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_size);
        if (client_fd == -1)
        {
            perror("accept");
            continue;
        }

        printf("Client connected.\n");

        // Réception de l'identifiant du client
        bytes_read = recv(client_fd, buffer, BUFSIZ - 1, 0);
        if (bytes_read <= 0)
        {
            perror("recv");
            close(client_fd);
            continue;
        }

        buffer[bytes_read] = '\0'; // Terminer la chaîne reçue
        printf("Client identifier received: \"%s\"\n", buffer);

        // Envoyer des commandes au client
        const char *commands[] = {"exfiltration", "fork", "out", "quit"};
        for (int i = 0; i < 4; i++)
        {
            printf("Sending command: %s\n", commands[i]);
            send(client_fd, commands[i], strlen(commands[i]), 0);
            sleep(1); // Simuler un délai entre les commandes
        }

        close(client_fd);
        printf("Client connection closed.\n");
    }

    close(socket_fd);
    return 0;
}
