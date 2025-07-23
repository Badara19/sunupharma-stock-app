#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "Products.h"
#include "Utils.h"
#include "Categories.h"
#define FICHIER_PRODUITS "../datas/PRODUCTS.dat"
#include <time.h>

Date dateAujourdHui() {
    Date d;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    d.jour = tm_info->tm_mday;
    d.mois = tm_info->tm_mon + 1;
    d.annee = tm_info->tm_year + 1900;
    return d;
}

int dateSuperieure(Date d1, Date d2) {
    if (d1.annee > d2.annee) return 1;
    if (d1.annee < d2.annee) return 0;
    if (d1.mois > d2.mois) return 1;
    if (d1.mois < d2.mois) return 0;
    if (d1.jour > d2.jour) return 1;
    return 0;
}

Date demanderDatePeremption() {
    Date d;
    int valide = 0;
    do {
        printf("Date de péremption (jj mm aaaa) : ");
        if (scanf("%d %d %d", &d.jour, &d.mois, &d.annee) == 3) {
            if (d.jour >= 1 && d.jour <= 31 && d.mois >= 1 && d.mois <= 12 && d.annee >= 2000) {
                valide = 1;
            } else {
                printf("Date invalide, recommencez.\n");
            }
        } else {
            printf("Format invalide, recommencez.\n");
            while (getchar() != '\n');
        }
    } while (!valide);
    while (getchar() != '\n');
    return d;
}

int ajouterMedicament() {
    FILE *f = fopen(FICHIER_PRODUITS, "ab+");
    if (!f) return 0;
    Medicament m, temp;
    char code[6];
    do {
        saisirChaine("Code (5 lettres) : ", code, sizeof(code));
        for (int i = 0; code[i]; i++) code[i] = toupper(code[i]);
        rewind(f);
        int existe = 0;
        while (fread(&temp, sizeof(Medicament), 1, f)) {
            if (strcmp(temp.code, code) == 0) {
                existe = 1;
                break;
            }
        }
        if (!existe) break;
    } while (1);

    strcpy(m.code, code);
    saisirChaine("Nom : ", m.nom, sizeof(m.nom));
    m.prixUnitaire = demanderPrix("Prix unitaire : ");
    m.quantite = demanderQuantite("Quantite : ");
    afficherCategories();
    int id;
    do {
        printf("ID categorie : ");
        scanf("%d", &id);
        while (getchar() != '\n');
    } while (!chercherCategorieParId(id, NULL));
    m.idCategorie = id;
    do {
        m.datePeremption = demanderDatePeremption();
    } while (!dateSuperieure(m.datePeremption, dateAujourdHui()));
    fwrite(&m, sizeof(Medicament), 1, f);
    fclose(f);
    return 1;
}

void afficherProduits() {
    FILE *f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) return;
    Medicament m;
    categorie c;
    printf("Code   Nom                               Prix      Qté   Catégorie        Péremption\n");
    while (fread(&m, sizeof(Medicament), 1, f)) {
        if (chercherCategorieParId(m.idCategorie, &c)) {
            printf("%-6s %-30s %-9.2f %-5d %-17s %02d/%02d/%04d\n", m.code, m.nom, m.prixUnitaire, m.quantite, c.nom, m.datePeremption.jour, m.datePeremption.mois, m.datePeremption.annee);
        }
    }
    fclose(f);
}

void rechercherMedicamentParCategorie() {
    char nomCat[30];
    saisirChaine("Nom categorie : ", nomCat, sizeof(nomCat));
    categorie cat;
    if (!chercherCategorieParNom(nomCat, &cat)) return;
    FILE *f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) return;
    Medicament m;
    Date ajd = dateAujourdHui();
    afficherEnteteStock();
    while (fread(&m, sizeof(Medicament), 1, f)) {
        if (m.idCategorie == cat.id) afficherLigneStock(m, ajd);
    }
    fclose(f);
}

int chercherMedicamentParCode(const char *code, Medicament *resultat) {
    FILE *f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) return 0;
    Medicament m;
    while (fread(&m, sizeof(Medicament), 1, f)) {
        if (strcmp(m.code, code) == 0) {
            if (resultat) *resultat = m;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int miseAJourStock(const char *code, int quantiteVendue) {
    FILE *f = fopen(FICHIER_PRODUITS, "rb+");
    if (!f) return 0;
    Medicament m;
    while (fread(&m, sizeof(Medicament), 1, f)) {
        if (strcmp(m.code, code) == 0) {
            if (m.quantite < quantiteVendue) {
                fclose(f);
                return 0;
            }
            m.quantite -= quantiteVendue;
            fseek(f, -sizeof(Medicament), SEEK_CUR);
            fwrite(&m, sizeof(Medicament), 1, f);
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void afficherEnteteStock() {
    printf("Code   Nom                               Prix      Qté   Péremption\n");
}

void afficherLigneStock(Medicament m, Date ajd) {
    printf("%-6s %-30s %-9.2f %-5d %02d/%02d/%04d\n", m.code, m.nom, m.prixUnitaire, m.quantite, m.datePeremption.jour, m.datePeremption.mois, m.datePeremption.annee);
}
