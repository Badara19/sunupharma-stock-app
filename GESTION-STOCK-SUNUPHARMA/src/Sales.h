#ifndef SALES_H
#define SALES_H

#include "Users.h"
#include "Products.h"

#define ID_VENTE_TAILLE 15

typedef struct {
    char idVente[ID_VENTE_TAILLE];
    char codeMedicament[6];
    char designation[50];
    int quantite;
    float prixUnitaire;
    float total;
    Date dateVente;
    char vendeur[30];
} Vente;

void effectuerVente(user vendeur);
void enregistrerVente(Vente v);
void afficherFacture(Vente v);
void menuGestionVentes();
void afficherVentesDuJour();
void afficherToutesVentes();
float chiffreAffairesJournalier();

#endif
