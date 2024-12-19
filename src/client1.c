#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 4242 // le port du serveur

int main(int argc, char **argv)
{
    struct sockaddr_in sa;
    int socket_fd;
    int status;
    char buffer[BUFSIZ];
    char msg[BUFSIZ];
    int bytes_sent, bytes_read;

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

    // Communication avec le serveur
    while (1)
    {
        printf("Enter a message (type 'quit' to exit): ");
        fgets(msg, BUFSIZ, stdin);
        msg[strcspn(msg, "\n")] = '\0'; 

        if (strcmp(msg, "quit") == 0)
        {
            printf("Disconnecting...\n");
            break;
        }

        bytes_sent = send(socket_fd, msg, strlen(msg), 0);
        if (bytes_sent == -1)
        {
            perror("send");
            break;
        }

        bytes_read = recv(socket_fd, buffer, BUFSIZ - 1, 0);
        if (bytes_read == 0)
        {
            printf("Server disconnected.\n");
            break;
        }
        else if (bytes_read == -1)
        {
            perror("recv");
            break;
        }
        else
        {
            buffer[bytes_read] = '\0';
            printf("Server response: \"%s\"\n", buffer);
        }
    }

    close(socket_fd);
    return 0;
}
