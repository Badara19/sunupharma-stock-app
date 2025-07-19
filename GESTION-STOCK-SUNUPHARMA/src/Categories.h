#ifndef CATEGORIES_H
#define CATEGORIES_H

#define MAX_NOM_CATEGORIE 30

typedef struct {
    int id;
    char nom[MAX_NOM_CATEGORIE];
} categorie;

void menuGestionCategories();
int ajouterCategorie();
void afficherCategories();
int supprimerCategorie(int id);
int chercherCategorieParId(int id, categorie *c);
int modifierCategorie(int id);
int chercherCategorieParNom(const char *nom, categorie *c);

#endif
