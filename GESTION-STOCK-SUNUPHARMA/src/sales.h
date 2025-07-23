#ifndef SALES_H
#define SALES_H

#include "products.h"
#include "users.h"

#define FICHIER_VENTES "../datas/sales.dat"
#define MAX_DATE 11
#define MAX_NUM_VENTE 20

typedef struct {
    char numero_vente[MAX_NUM_VENTE];
    int idProduit;
    int idUser;
    int quantite;
    char date[MAX_DATE];
    float total_vente;
} sale;

int genererIdVente();
int enregistrerVente(int idUser, int idProduit, int quantite);
void afficherToutesVentes();
void afficherVentesDuJour();
float chiffreAffairesDuJour();

#endif
