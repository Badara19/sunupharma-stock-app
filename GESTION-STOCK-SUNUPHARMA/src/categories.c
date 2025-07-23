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

#define FICHIER_CATEGORIES "../datas/categories.dat"

int genererIdCategorie() {
    FILE* f = fopen(FICHIER_CATEGORIES, "rb");
    if (!f) return 1;
    categorie c;
    int id = 0;
    while (fread(&c, sizeof(categorie), 1, f)) {
        if (c.id > id) id = c.id;
    }
    fclose(f);
    return id + 1;
}

int existeCategorie(int id) {
    FILE* f = fopen(FICHIER_CATEGORIES, "rb");
    if (!f) return 0;
    categorie c;
    while (fread(&c, sizeof(categorie), 1, f)) {
        if (c.id == id) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int ajouterCategorie() {
    puts("");
    color_header("********** AJOUT DE CATEGORIE **********\n");
    puts("");
    categorie c;
    c.id = genererIdCategorie();

    printf("Nom de la categorie : \n");
    lireChaine(c.nom, MAX_NOM);

    FILE* f = fopen(FICHIER_CATEGORIES, "ab");
    if (!f) {
        color_error("Erreur ouverture fichier\n");
        return 0;
    }

    fwrite(&c, sizeof(categorie), 1, f);
    fclose(f);
    color_success("Categorie ajoutee avec succes\n");
    return 1;
}

void afficherToutesCategories() {
    puts("");
    color_header("********** AFFICHAGE DE TOUS LES CATEGORIES **********\n");
    puts("");
    FILE* f = fopen(FICHIER_CATEGORIES, "rb");
    if (!f) {
        color_error("Aucune categorie trouvee\n");
        return;
    }

    categorie c;
    printf("\n%-5s %-20s\n", "ID", "Nom");
    while (fread(&c, sizeof(categorie), 1, f)) {
        printf("%-5d %-20s\n", c.id, c.nom);
    }

    fclose(f);
}

void menuGestionCategories() {
    pauseEtNettoie();
    int choix;
    do {
        puts("");
        color_title("\nMENU GESTION DES CATEGORIES\n");
        puts("");
        printf("1. Ajouter une categorie\n");
        printf("2. Afficher les categories\n");
        printf("0. Retour\n");

        choix = saisirEntierAvecMessage("Votre choix : ", 0, 2);
        switch (choix) {
            case 1: ajouterCategorie(); break;
            case 2: afficherToutesCategories(); break;
            case 0: break;
        }
    } while (choix != 0);
}

char* getNomCategorie(int idCategorie) {
    static char nom[50];
    FILE* f = fopen(FICHIER_CATEGORIES, "rb");
    if (!f) return NULL;

    categorie c;
    while (fread(&c, sizeof(categorie), 1, f)) {
        if (c.id == idCategorie) {
            fclose(f);
            strcpy(nom, c.nom);
            return nom;
        }
    }

    fclose(f);
    return NULL;
}
