#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "colors.h"
#include "users.h"
#include "products.h"
#include "categories.h"
#include "sales.h"
#include "rapports.h"

void lireChaine(char* buffer, int taille) {
    if (fgets(buffer, taille, stdin)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\0') {
            buffer[len - 1] = '\0';
        }
    }
}

int saisirEntier(int min, int max) {
    int val;
    char c;
    while (1) {
        if (scanf("%d", &val) != 1) {
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Entrez un entier : ");
        } else if (val < min || val > max) {
            printf("Entier hors limites (%d - %d) : ", min, max);
            viderBuffer();
        } else {
            viderBuffer();
            return val;
        }
    }
}

int saisirEntierAvecMessage(const char* msg, int min, int max) {
    int val;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &val) != 1) {
            viderBuffer();
            printf("Entree invalide\n");
        } else if (val < min || val > max) {
            viderBuffer();
            printf("Entier hors limites (%d - %d)\n", min, max);
        } else {
            viderBuffer();
            return val;
        }
    }
}

void viderBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pauseEtNettoie(void) {
    printf("\nAppuyez sur Entree pour continuer...");
    getchar();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void verifFichier(FILE *fichier) {
    if (fichier == NULL) {
        perror("Erreur ouverture fichier");
        exit(EXIT_FAILURE);
    }
}
