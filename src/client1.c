#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 4242 // le port du serveur

// Fonction pour obtenir l'identifiant de la machine
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
    snprintf(identifier, size, "%s-%s", hostname, ip_address);
}

// Fonction pour gérer les commandes reçues du serveur
void handle_client_commands(int server_fd)
{
    char buffer[BUFSIZ];
    int bytes_read;

    while (1)
    {
        // Recevoir une commande du serveur
        bytes_read = recv(server_fd, buffer, BUFSIZ - 1, 0);
        if (bytes_read <= 0)
        {
            if (bytes_read == 0)
                printf("Server disconnected.\n");
            else
                perror("recv");
            break;
        }

        buffer[bytes_read] = '\0'; // Terminer la chaîne reçue
        printf("Command received from server: \"%s\"\n", buffer);

        // Gestion des commandes spécifiques
        if (strcmp(buffer, "exfiltration") == 0)
        {
            printf("Executing exfiltration...\n");
        }
        else if (strcmp(buffer, "fork") == 0)
        {
            printf("Executing fork...\n");
        }
        else if (strcmp(buffer, "out") == 0)
        {
            printf("Executing out...\n");
        }
        else if (strcmp(buffer, "quit") == 0)
        {
            printf("Server requested disconnection.\n");
            break;
        }
        else
        {
            printf("Unknown command: \"%s\"\n", buffer);
        }
    }
}

int main(int argc, char **argv)
{
    struct sockaddr_in sa;
    int socket_fd;
    int status;
    char identifier[BUFSIZ];

    // Préparation de l'adresse et du port
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;                     // IPv4
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    sa.sin_port = htons(PORT);

    // Création de la socket et connexion au serveur
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

    // Envoi de l'identifiant de la machine
    get_machine_identifier(identifier, sizeof(identifier));
    printf("Sending machine identifier: %s\n", identifier);

    if (send(socket_fd, identifier, strlen(identifier), 0) == -1)
    {
        perror("send");
        close(socket_fd);
        return 3;
    }

    // Gestion des commandes du serveur
    handle_client_commands(socket_fd);

    close(socket_fd);
    return 0;
}
