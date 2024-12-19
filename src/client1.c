#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "handle_client_commands.h" // Déclare la fonction

#define PORT 4242 // le port du serveur

int main()
{
    struct sockaddr_in sa;
    int socket_fd, status;
    char identifier[BUFSIZ];

    // Configuration de la connexion au serveur
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    sa.sin_port = htons(PORT);

    socket_fd = socket(sa.sin_family, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("socket");
        return 1;
    }
    status = connect(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status != 0)
    {
        perror("connect");
        close(socket_fd);
        return 2;
    }

    // Envoi d'un identifiant fictif
    snprintf(identifier, sizeof(identifier), "hostname-127.0.0.1");
    if (send(socket_fd, identifier, strlen(identifier), 0) == -1)
    {
        perror("send");
        close(socket_fd);
        return 3;
    }

    // Utiliser la fonction `handle_client_commands` pour gérer les réponses
    handle_client_commands(socket_fd);

    close(socket_fd);
    return 0;
}
