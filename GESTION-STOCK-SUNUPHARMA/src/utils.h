#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>

void lireChaine(char* buffer, int taille);
int saisirEntier(int min, int max);
int saisirEntierAvecMessage(const char* msg, int min, int max);
void viderBuffer(void);
void pauseEtNettoie(void);
void verifFichier(FILE *fichier);

#endif
