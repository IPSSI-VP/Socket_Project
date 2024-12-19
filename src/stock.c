#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>


char nom_fichier[] = "LOGS.txt";
unsigned int longueur;
char *stock(char *liste)
{

    FILE *p_fichier = fopen(nom_fichier, "a"); /* pointeur sur fichier*/
    size_t result;

    printf("%s\n", liste);
    longueur = strlen(liste);
    fwrite(liste, sizeof(char), longueur, p_fichier);
    fputc('\n', p_fichier);
    

    free(liste);
    fclose(p_fichier);
    return 0;
}
