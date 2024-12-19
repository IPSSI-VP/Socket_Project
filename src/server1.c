#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "handle_client_commands.h"  // Inclure le fichier d'en-tête

#define PORT 4242  // le port du serveur
#define BACKLOG 10 // nombre max de demandes de connexion

int main()
{
    struct sockaddr_in sa;
    int socket_fd;
    int client_fd;
    int status;
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
    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status != 0)
    {
        perror("bind");
        close(socket_fd);
        return 2;
    }

    // Écouter les connexions entrantes
    status = listen(socket_fd, BACKLOG);
    if (status != 0)
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

        // Envoyer un accusé de réception au client
        const char *ack_msg = "Identifier received.";
        send(client_fd, ack_msg, strlen(ack_msg), 0);

        // Appel de la fonction pour traiter les commandes
        handle_client_commands(client_fd);

        close(client_fd);
        printf("Client connection closed.\n");
    }

    close(socket_fd);
    return 0;
}
