#ifndef CATEGORIES_H
#define CATEGORIES_H

#define FICHIER_CATEGORIES "datas/categories.dat"
#define MAX_NOM 50

typedef struct {
    int id;
    char nom[MAX_NOM];
} categorie;

int genererIdCategorie();
int existeCategorie(int id);
void afficherToutesCategories();
int ajouterCategorie();
void menuGestionCategories();
char* getNomCategorie(int idCategorie);

#endif
