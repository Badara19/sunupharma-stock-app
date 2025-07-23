#ifndef PRODUCTS_H
#define PRODUCTS_H

#include "Categories.h"
#include "Utils.h"

typedef struct {
    int jour;
    int mois;
    int annee;
} Date;

typedef struct {
    char code[6];
    char nom[50];
    float prixUnitaire;
    int quantite;
    int idCategorie;
    Date datePeremption;
} Medicament;

Date dateAujourdHui();
int dateSuperieure(Date d1, Date d2);
Date demanderDatePeremption();
int ajouterMedicament();
void afficherProduits();
void rechercherMedicamentParCategorie();
int chercherMedicamentParCode(const char *code, Medicament *resultat);
int miseAJourStock(const char *code, int quantiteVendue);
void afficherEnteteStock();
void afficherLigneStock(Medicament m, Date ajd);

#endif
