#ifndef SALES_H
#define SALES_H

#define MAX_PRODUITS 100

typedef struct {
    char code[20];
    char designation[50];
    int quantite;
    float prix_unitaire;
    float total;
} ProduitVendu;

typedef struct {
    char nom_pharmacie[100];
    char numero_vente[18];
    char login[100];
    int nb_produits;
    ProduitVendu produits[MAX_PRODUITS];
    float total_vente;
} Vente;

typedef struct {
    char code[6];
    char designation[50];
    float prix;
    int quantite_stock;
    char categorie[50];
    char date_peremption[11];
} Produit;

void afficherMenuVente();
void initVente(Vente *vente, const char *loginPharmacien);
void calculerTotalVente(Vente *vente);
void mettreAJourStock(Produit stock[], int nbProduitsStock, Vente vente);
void genererFacture(Vente *vente, const char *idPharmacien);
void afficherMessage(const char *message, int estErreur);
void ajouterProduitVente(Produit stock[], int nbProduits);

#endif
