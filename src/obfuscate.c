#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "gener.h"
#include "stock.h"

// Définir le jeu de caractères pour générer une chaîne alphanumérique aléatoire
#define CHARSET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

void obfuscateFile(const char *filename)
{
    // Taille d'un bloc
    size_t blockSize = 53;
    char ch;

    FILE *file = fopen(filename, "r+"); // Ouvrir le fichier en mode lecture/écriture
    if (!file)
    {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return;
    }

    /*  // Calculer la taille du fichier
     fseek(file, 0, SEEK_END);
     size_t fileSize = ftell(file);
     printf("fileSize: %d",fileSize);
     rewind(file); */

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

        // Générer une chaîne aléatoire de même longueur que le bloc lu
        char *randomString = gener(bytesRead);
        /*printf("Chaîne aléatoire générée : %s\n", randomString);*/
        // Appliquer XOR sur chaque caractère du bloc
        for (int i = 0; i < bytesRead; i++)
        {
            content[i] = content[i] ^ randomString[i];

            /*printf("Bloc : %s\n", content);*/
        }
        // Repositionner le curseur pour écrire les données modifiées
        /*fseek(file, -(long)bytesRead, SEEK_CUR);
        fwrite(content, sizeof(char), bytesRead, file);*/
        stock(randomString);
        free(randomString);
    }

    free(content);

    fclose(file);
}
int main()
{
    setlocale(LC_ALL, "fr_FR.UTF-8");
    const char *filename = "personne.txt"; // Nom du fichier à traiter

    // Appliquer l'obfuscation XOR sur le fichier
    obfuscateFile(filename);

    printf("Fichier obfusqué avec succès.\n");
    return 0;
}