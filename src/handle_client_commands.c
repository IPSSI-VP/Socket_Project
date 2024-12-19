#include <stdio.h>
#include <string.h>
#include <sys/socket.h> // Pour recv et send

void handle_client_commands(int server_fd)
{
    char buffer[BUFSIZ];
    int bytes_read;

    while (1)
    {
        // Recevoir les commandes du serveur
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
            printf("Disconnecting as per server request.\n");
            break;
        }
        else
        {
            printf("Unknown command: \"%s\"\n", buffer);
        }
    }
}
