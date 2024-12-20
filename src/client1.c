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

void get_machine_identifier(char *identifier, size_t size)
{
    char hostname[256];
    char ip_address[INET_ADDRSTRLEN];

    // Récupération du hostname
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        perror("gethostname");
        snprintf(hostname, sizeof(hostname), "unknown");
    }

    // Récupération de l'adresse IP
    struct hostent *host = gethostbyname(hostname);

if (host && host->h_addr_list[0])
{
    inet_ntop(AF_INET, host->h_addr_list[0], ip_address, sizeof(ip_address));
}
else
{
    snprintf(ip_address, sizeof(ip_address), "127.0.0.1");
}

// Construction de l'identifiant
snprintf(identifier, size, "%s-%s", hostname, ip_address);}

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
    get_machine_identifier(identifier, sizeof(identifier));
    printf("Sending machine identifier: %s\n", identifier);

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
