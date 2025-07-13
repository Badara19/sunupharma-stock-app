#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "products.h"
#define MAX 100

const char *nomFichier = "PRODUCTS.dat";

void verification(FILE *fp) {
    if (fp == NULL) {
        printf("Fichier introuvable\n");
        exit(1);
    }
}

int recupereIdCategorie() {
    FILE *f;
    struct categorie c;
    int dernierId = 0;

    f = fopen("CATEGORIES.dat", "rb");
    if (f == NULL) {
        return 0;
    }

    while (fread(&c, sizeof(struct categorie), 1, f) == 1) {
        dernierId = c.idCat;
    }

    fclose(f);
    return dernierId;
}

void saisiecategories(struct categorie *c, int *nbCat) {
    FILE *s = fopen("CATEGORIES.dat", "ab+");
    verification(s);

    printf("Saisir le nombre de categories : ");
    scanf("%d", nbCat);
    getchar();

    for (int i = 0; i < *nbCat; i++) {
        c[i].idCat = recupereIdCategorie() + 1;

        printf("Identifiant de la categorie %d : %d\n", i + 1, c[i].idCat);
        printf("Saisir le libelle de la categorie : ");
        fgets(c[i].libelleCat, sizeof(c[i].libelleCat), stdin);
        c[i].libelleCat[strcspn(c[i].libelleCat, "\n")] = 0;
        fwrite(&c[i], sizeof(struct categorie), 1, s);
    }
    fclose(s);
}

void afficherCategories() {
    struct categorie c;
    FILE *f = fopen("CATEGORIES.dat", "rb");
    verification(f);

    printf("\n--- Liste des categories ---\n");
    while (fread(&c, sizeof(struct categorie), 1, f) == 1) {
        printf("ID : %d\nLibelle : %s\n---------------------------\n", c.idCat, c.libelleCat);
    }
    fclose(f);
}

void modifierCategorie() {
    FILE *f = fopen("CATEGORIES.dat", "r+b");
    verification(f);

    int idRecherche;
    struct categorie c;
    int trouve = 0;

    printf("Entrez l'ID de la categorie a modifier : ");
    scanf("%d", &idRecherche);
    getchar();

    while (fread(&c, sizeof(struct categorie), 1, f) == 1) {
        if (c.idCat == idRecherche) {
            trouve = 1;
            printf("Categorie trouvee. Nouveau libelle : ");
            fgets(c.libelleCat, sizeof(c.libelleCat), stdin);
            c.libelleCat[strcspn(c.libelleCat, "\n")] = 0;
            fseek(f, -sizeof(struct categorie), SEEK_CUR);
            fwrite(&c, sizeof(struct categorie), 1, f);
            printf("Categorie modifiee avec succes.\n");
            break;
        }
    }

    if (!trouve) {
        printf("Categorie non trouvee.\n");
    }
    fclose(f);
}

void supprimerCategorie() {
    FILE *f = fopen("CATEGORIES.dat", "rb");
    FILE *tmp = fopen("TMP_CATEGORIES.dat", "wb");
    verification(f);
    verification(tmp);

    int idRecherche;
    struct categorie c;
    int trouve = 0;

    printf("Entrez l'ID de la categorie a supprimer : ");
    scanf("%d", &idRecherche);

    while (fread(&c, sizeof(struct categorie), 1, f) == 1) {
        if (c.idCat != idRecherche) {
            fwrite(&c, sizeof(struct categorie), 1, tmp);
        } else {
            trouve = 1;
        }
    }

    fclose(f);
    fclose(tmp);

    if (trouve) {
        remove("CATEGORIES.dat");
        rename("TMP_CATEGORIES.dat", "CATEGORIES.dat");
        printf("Categorie supprimee avec succes.\n");
    } else {
        remove("TMP_CATEGORIES.dat");
        printf("Categorie non trouvee.\n");
    }
}

void ajoutProduits(struct produits *p, int *nbProd, struct categorie *c, int nbCat) {
    FILE *fp = fopen(nomFichier, "a+b");
    verification(fp);

    printf("\nSaisir le nombre de produits : ");
    scanf("%d", nbProd);
    getchar();

    for (int i = 0; i < *nbProd; i++) {
        do {
            printf("Saisir un code de 5 caracteres : ");
            fgets(p[i].code, sizeof(p[i].code), stdin);
            p[i].code[strcspn(p[i].code, "\n")] = 0;

            if (strlen(p[i].code) != 5) {
                printf("Code invalide. Il doit contenir exactement 5 caracteres.\n");
            }

            int existe = 0;
            FILE *check = fopen(nomFichier, "rb");
            struct produits temp;
            while (fread(&temp, sizeof(struct produits), 1, check) == 1) {
                if (strcmp(temp.code, p[i].code) == 0) {
                    existe = 1;
                    break;
                }
            }
            fclose(check);

            if (existe) {
                printf("Code produit deja existant. Produit non ajoute.\n");
                i--;
                continue;
            }

        } while (strlen(p[i].code) != 5);

        printf("Saisir la designation du produit : ");
        fgets(p[i].designation, sizeof(p[i].designation), stdin);
        p[i].designation[strcspn(p[i].designation, "\n")] = 0;

        printf("Saisir le prix : ");
        scanf("%d", &p[i].prix);
        printf("Saisir la quantite : ");
        scanf("%d", &p[i].quantiteStock);
        getchar();

        printf("Saisir la date de peremption : ");
        fgets(p[i].datePeremption, sizeof(p[i].datePeremption), stdin);
        p[i].datePeremption[strcspn(p[i].datePeremption, "\n")] = 0;

        printf("Saisir l'identifiant de la categorie : ");
        scanf("%d", &p[i].idCat);
        getchar();

        fwrite(&p[i], sizeof(struct produits), 1, fp);

        int trouve = 0;
        for (int j = 0; j < nbCat; j++) {
            if (p[i].idCat == c[j].idCat) {
                printf("Libelle categorie : %s\n", c[j].libelleCat);
                trouve = 1;
                break;
            }
        }
        if (!trouve) {
            printf("Identifiant de categorie inexistant !\n");
        }
    }
    fclose(fp);
}

void afficheProduit() {
    FILE *f = fopen("PRODUCTS.dat", "rb");
    verification(f);
    struct produits p;

    printf("\n--- Liste des produits ---\n");
    while (fread(&p, sizeof(struct produits), 1, f) == 1) {
        printf(" Code : %s\n Designation : %s\n Prix : %d CFA\n Stock : %d\n Date Expiration : %s\n ID Categorie : %d\n-----------------------------\n",
               p.code, p.designation, p.prix, p.quantiteStock, p.datePeremption, p.idCat);
    }
    fclose(f);
}

void modifierProduit(const char *nomFichier) {
    FILE *f = fopen(nomFichier, "r+b");
    verification(f);

    char codeRecherche[6];
    int trouve = 0;

    printf("Entrer le code du produit a modifier : ");
    fgets(codeRecherche, sizeof(codeRecherche), stdin);
    codeRecherche[strcspn(codeRecherche, "\n")] = 0;

    struct produits p;

    while (fread(&p, sizeof(struct produits), 1, f) == 1) {
        if (strcmp(p.code, codeRecherche) == 0) {
            trouve = 1;
            printf("Produit trouve. Modifier les informations :\n");

            printf("Nouveau code : ");
            fgets(p.code, sizeof(p.code), stdin);
            p.code[strcspn(p.code, "\n")] = 0;

            printf("Nouvelle designation : ");
            fgets(p.designation, sizeof(p.designation), stdin);
            p.designation[strcspn(p.designation, "\n")] = 0;

            printf("Nouveau prix : ");
            scanf("%d", &p.prix);
            printf("Nouvelle quantite : ");
            scanf("%d", &p.quantiteStock);
            getchar();

            printf("Nouvelle date de peremption : ");
            fgets(p.datePeremption, sizeof(p.datePeremption), stdin);
            p.datePeremption[strcspn(p.datePeremption, "\n")] = 0;

            fseek(f, -sizeof(struct produits), SEEK_CUR);
            fwrite(&p, sizeof(struct produits), 1, f);
            printf("Produit modifie avec succes.\n");
            break;
        }
    }

    if (!trouve) {
        printf("Produit introuvable.\n");
    }
    fclose(f);
}

void supprimerProduit(const char *nomFichier) {
    FILE *original = fopen(nomFichier, "rb");
    FILE *temporaire = fopen("TEMP.dat", "w+b");
    verification(original);
    verification(temporaire);

    char codeRecherche[6];
    int trouve = 0;
    struct produits p;

    printf("Entrez le code du produit a supprimer : ");
    fgets(codeRecherche, sizeof(codeRecherche), stdin);
    codeRecherche[strcspn(codeRecherche, "\n")] = 0;

    while (fread(&p, sizeof(struct produits), 1, original) == 1) {
        if (strcmp(p.code, codeRecherche) != 0) {
            fwrite(&p, sizeof(struct produits), 1, temporaire);
        } else {
            trouve = 1;
        }
    }
    fclose(original);
    fclose(temporaire);

    if (trouve) {
        remove(nomFichier);
        rename("TEMP.dat", nomFichier);
        printf("Produit supprime avec succes.\n");
    } else {
        remove("TEMP.dat");
        printf("Produit non trouve.\n");
    }
}

void rechercherProduitParCode() {
    FILE *f = fopen("PRODUCTS.dat", "rb");
   verification(f);
    char codeRecherche[6];
    printf("Entrez le code du produit a rechercher (5 caracteres) : ");
    fgets(codeRecherche, sizeof(codeRecherche), stdin);
    codeRecherche[strcspn(codeRecherche, "\n")] = 0; // Enlever le '\n'

    struct produits p;
    int trouve = 0;

    while (fread(&p, sizeof(struct produits), 1, f) == 1) {
        if (strcmp(p.code, codeRecherche) == 0) {
            printf("\nProduit trouve :\n");
            printf(" Code : %s\n", p.code);
            printf(" Designation : %s\n", p.designation);
            printf(" Prix : %d CFA\n", p.prix);
            printf(" Stock : %d\n", p.quantiteStock);
            printf(" Date Expiration : %s\n", p.datePeremption);
            printf(" ID Categorie : %d\n", p.idCat);
            trouve = 1;
            break;
        }
    }

    if (!trouve) {
        printf("Produit non trouve avec ce code.\n");
    }

    fclose(f);
}
void menu() {
    int choix;
    int nbProd = 0, nbCat = 0;
    struct produits produits[MAX];
    struct categorie categories[MAX];

    do {
        printf("\n===== MENU =====\n");
        printf("1. Saisir categories\n");
        printf("2. Afficher categories\n");
        printf("3. Modifier categorie\n");
        printf("4. Supprimer categorie\n");
        printf("5. Ajouter produit\n");
        printf("6. Afficher produits\n");
        printf("7. Modifier produit\n");
        printf("8. Supprimer produit\n");
        printf("9. Rechercher un produit\n");
        printf("10. Quitter\n");
        printf("==================\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        getchar();

        switch (choix) {
            case 1: saisiecategories(categories, &nbCat); break;
            case 2: afficherCategories(); break;
            case 3: modifierCategorie(); break;
            case 4: supprimerCategorie(); break;
            case 5: {
                FILE *fc = fopen("CATEGORIES.dat", "rb");
                verification(fc);
                nbCat = fread(categories, sizeof(struct categorie), MAX, fc);
                fclose(fc);
                ajoutProduits(produits, &nbProd, categories, nbCat);
                break;
            }
            case 6: afficheProduit(); break;
            case 7: modifierProduit(nomFichier); break;
            case 8: supprimerProduit(nomFichier); break;
            case 9: rechercherProduitParCode();break;
            case 10: printf("Fin du programme.\n"); break;
            default: printf("Choix invalide.\n");
        }
    } while (choix != 10);
}
