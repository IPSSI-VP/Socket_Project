#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Fonction pour traiter les commandes envoyées par le client
void handle_client_commands(int client_fd)
{
    char buffer[BUFSIZ];
    int bytes_read;

    // Boucle pour lire les commandes du client
    while (1)
    {
        // Réception des commandes du client
        bytes_read = recv(client_fd, buffer, BUFSIZ - 1, 0);
        if (bytes_read <= 0)
        {
            if (bytes_read == 0)
                printf("Client disconnected.\n");
            else
                perror("recv");
            break;
        }

        buffer[bytes_read] = '\0'; // Terminer la chaîne reçue
        printf("Command received: \"%s\"\n", buffer);

        // Gestion des ordres spécifiques
        if (strcmp(buffer, "exfiltration") == 0)
        {
            printf("Executing exfiltration command...\n");
            send(client_fd, "Exfiltration executed.", 23, 0);
        }
        else if (strcmp(buffer, "fork") == 0)
        {
            printf("Executing fork command...\n");
            send(client_fd, "Fork executed.", 15, 0);
        }
        else if (strcmp(buffer, "out") == 0)
        {
            printf("Executing out command...\n");
            send(client_fd, "Out executed.", 14, 0);
        }
        else if (strcmp(buffer, "quit") == 0)
        {
            printf("Client requested disconnection.\n");
            send(client_fd, "Goodbye.", 8, 0);
            break;
        }
        else
        {
            printf("Unknown command: \"%s\"\n", buffer);
            send(client_fd, "Unknown command.", 17, 0);
        }
    }
}
