#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>


char nom_fichier[] = "LOGS.txt";
unsigned int longueur;
unsigned int longueur_uid;
char current_time[20]; // Format YYYY-MM-DD HH:MM:SS

char *stock(char *liste, char *user_id)
{

    FILE *p_fichier = fopen(nom_fichier, "a"); /* pointeur sur fichier*/
    size_t result;

    printf("%s\n", liste);
    longueur = strlen(liste);
    fwrite(liste, sizeof(char), longueur, p_fichier);
    fputc(' ', p_fichier); // Écrire un espace en hexadécimal (0x20)

    /* Écrire la chaîne en hexadécimal */
    for (size_t i = 0; i < strlen(liste); i++)
    {
        fprintf(p_fichier, "%02X", (unsigned char)liste[i]); // Format hexadécimal
    }
    fprintf(p_fichier, " User = ");
    longueur_uid = strlen(user_id);
    fwrite(user_id, sizeof(char), longueur_uid, p_fichier);

    time_t now;
    struct tm *time_info;

    // Récupère l'heure actuelle
    time(&now);

    // Convertit en une structure tm pour la manipulation
    time_info = localtime(&now);
    // Formate l'heure et la stocke dans la variable
    strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", time_info);
    fprintf(p_fichier, " Time = ");
    fwrite(current_time, sizeof(char), 19, p_fichier);
    fputc('\n', p_fichier);

    free(liste);
    fclose(p_fichier);
    return 0;
}
