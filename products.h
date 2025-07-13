#ifndef PRODUITS_H_INCLUDED
#define PRODUITS_H_INCLUDED
struct categorie {
    int idCat;
    char libelleCat[50];
};

struct produits {
    char code[6];
    char designation[50];
    int prix;
    int quantiteStock;
    char datePeremption[20];
    int idCat;
};
int recupereIdCategorie();
void verification();
void ajoutProduits(struct produits *p, int *nbProd, struct categorie *c, int nbCat);
void saisiecategories(struct categorie *c, int *nbCat);
 void afficherCategories();
void modifierCategorie();
void supprimerCategorie();
void modifierProduit();
void supprimerProduit();
void afficheProduit();
void rechercherProduitParCode();
void menu();

#endif // PRODUITS_H_INCLUDED
