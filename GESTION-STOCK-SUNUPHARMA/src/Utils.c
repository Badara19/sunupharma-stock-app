#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Utils.h"
#include "Colors.h"

int confirmation(const char *message) {
    char c;
    printf("%s", message);
    c = getchar();
    while (getchar() != '\n');
    return (c == 'O' || c == 'o');
}

void pauseEtNettoie() {
    printf("Appuyez sur Entrée pour continuer...");
    while (getchar() != '\n');
}

void saisirChaine(const char* invite, char* chaine, int taille) {
    printf("%s", invite);
    if (fgets(chaine, taille, stdin)) {
        size_t len = strlen(chaine);
        if (len > 0 && chaine[len-1] == '\n') {
            chaine[len-1] = '\0';
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}

float demanderPrix(const char *invite) {
    float prix;
    int valide = 0;
    do {
        printf("%s", invite);
        if (scanf("%f", &prix) == 1 && prix >= 0) {
            valide = 1;
        } else {
            printf("Prix invalide, recommencez.\n");
            while (getchar() != '\n');
        }
    } while (!valide);
    while (getchar() != '\n');
    return prix;
}

int demanderQuantite(const char *invite) {
    int quantite;
    int valide = 0;
    do {
        printf("%s", invite);
        if (scanf("%d", &quantite) == 1 && quantite >= 0) {
            valide = 1;
        } else {
            printf("Quantite invalide, recommencez.\n");
            while (getchar() != '\n');
        }
    } while (!valide);
    while (getchar() != '\n');
    return quantite;
}
