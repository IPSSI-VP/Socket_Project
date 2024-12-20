#ifndef EXFILTRATION_H
#define EXFILTRATION_H

#include <stdio.h>
#include <stdlib.h>

// Fonction pour exfiltrer le contenu du fichier
// Elle lit le contenu du fichier "loreem.txt" et l'affiche en blocs de taille spécifiée.
void exfiltration(int socket_fd);

#endif // EXFILTRATION_H
