#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define CHARSET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

// Fonction pour générer une chaîne alphanumérique aléatoire
char *gener(unsigned int length)
{
    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));
    char *str = (char *)malloc((length + 1) * sizeof(char));

    if (str == NULL)
    {
        printf("Erreur d'allocation mémoire\n");
        exit(1);
    }
    // Remplir la chaîne avec des caractères aléatoires
    for (int i = 0; i < length; i++)
    {
        // Choisir un caractère aléatoire du jeu de caractères
        str[i] = CHARSET[rand() % (sizeof(CHARSET) - 1)];
    }

    // Ajouter le caractère de fin de chaîne
    str[length] = '\0';
    return str;
}
