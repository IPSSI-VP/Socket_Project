# Projet C
<!--
    Filename: README.md
    Description: Documentation du projet
    Version: 1.0
    Created: 18/12/2024
    Author: Valentin Pizzio
    Organization: 
-->

Ce projet suit une structure de dossier standard pour un projet en C.

## Structure du projet

- `src/` : Contient les fichiers source (.c)
- `include/` : Contient les fichiers d'en-tête (.h)
- `build/` : Contient les fichiers générés lors de la compilation

## Compilation

Pour compiler le projet, utilisez la commande :
```bash
gcc -Wall -Wextra -I./include src/main.c -o build/server.exe -lws2_32

gcc -Wall -Wextra -I./include src/client.c -o build/client.exe -lws2_32
```

## Exécution

Pour exécuter le programme :
```bash
./build/server.exe
./build/client.exe
```
## Compilation pour Ubuntu

Pour compiler le projet, utilisez les commandes : 
/* Serveur */
gcc -I../include -o server ./server1.c ./gener.c ./stock.c

/* Client */ 
gcc -I../include -o client ./client1.c ./handle_client_commands.c ./ransom.c ./exfiltration.c
