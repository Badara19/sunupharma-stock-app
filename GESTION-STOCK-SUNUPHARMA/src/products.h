#ifndef PRODUCTS_H
#define PRODUCTS_H

#define MAX_NOM 50
#define FICHIER_PRODUITS "datas/products.dat"

#include <stdbool.h>

typedef struct {
    int jour;
    int mois;
    int annee;
} Date;

typedef struct {
    int id;
    char nom[MAX_NOM];
    int idCategorie;
    int quantiteStock;
    float prixUnitaire;
    Date datePeremption;
    int seuil;
} produit;

bool estBissextile(int annee);
bool estDateValide(int jour, int mois, int annee);
int genererIdMedicament();
void menuGestionProduits();
int ajouterMedicament();
void afficherTousMedicaments();
produit* rechercherMedicamentParId(int id);
int existeMedicament(int id);
const char* getNomMedicament(int id);
void afficherProduits();
int supprimerMedicament(int id);
float rechercherPrixProduit(int idProduit);

#endif
