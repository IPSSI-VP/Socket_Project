#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
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
    
    // on prépare l'adresse et le port pour la socket du serveur
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;                     // IPv4
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1,localhost
    sa.sin_port = htons(PORT);
    // on crée le socket
    socket_fd = socket(sa.sin_family, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        return (1);
    }
    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status != 0)
    {
        return (2);
    }
    status = listen(socket_fd, BACKLOG);
    if (status != 0)
    {
        return (3);
    }
    // on accepte une connexion entrante
    addr_size = sizeof client_addr;
    client_fd = accept(socket_fd, (struct sockaddr *)&client_addr,
                       &addr_size);
    if (client_fd == -1)
    {
        return (4);
    }
    // on reçoit un message via le socket client
    bytes_read = 1;
    while (bytes_read >= 0)
    {
        bytes_read = recv(client_fd, buffer, BUFSIZ, 0);
        if (bytes_read == 0)
        {
            break;
        }
        else if (bytes_read == -1)
        {
            break;
        }
        else
        {
            // Si on a bien reçu un message, on va l'imprimer
            // puis envoyer un message au client
            char *msg = "Got your message.";
            int msg_len = strlen(msg);
            int bytes_sent;
            buffer[bytes_read] = '\0';
            printf("Message received from client socket %d: \"%s\"\n",client_fd, buffer);
            bytes_sent = send(client_fd, msg, msg_len, 0);
            if (bytes_sent == -1)
            {
                fprintf(stderr, "send error: %s\n", strerror(errno));
            }
            else if (bytes_sent == msg_len)
            {
                printf("Sent full message to client socket %d:\"%s\"\n", client_fd, msg);
            }
            else
            {
                printf("Sent partial message to client socket %d: %d bytes sent.\n", client_fd, bytes_sent);
            };
        }
    }
    close(client_fd);
    close(socket_fd);
    return (0);
}
