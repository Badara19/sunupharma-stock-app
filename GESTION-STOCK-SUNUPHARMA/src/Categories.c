#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Categories.h"
#include "Utils.h"
#include "Colors.h"

#define FICHIER_CATEGORIES "../datas/CATEGORIES.dat"

int ajouterCategorie() {
    FILE *f = fopen(FICHIER_CATEGORIES, "ab+");
    if (!f) {
        printf(COULEUR_ROUGE "[ERREUR] Impossible d'ouvrir %s\n" COULEUR_RESET, FICHIER_CATEGORIES);
        return 0;
    }

    categorie c;
    int maxId = 0;

    rewind(f);
    while (fread(&c, sizeof(categorie), 1, f) == 1) {
        if (c.id > maxId) maxId = c.id;
    }

    c.id = maxId + 1;

    printf("Nom de la nouvelle catégorie : ");
    if (fgets(c.nom, sizeof(c.nom), stdin) == NULL) {
        printf(COULEUR_ROUGE "[ERREUR] Lecture du nom impossible.\n" COULEUR_RESET);
        fclose(f);
        return 0;
    }
    c.nom[strcspn(c.nom, "\n")] = '\0';

    categorie tempCat;
    rewind(f);
    while (fread(&tempCat, sizeof(categorie), 1, f) == 1) {
        if (strcasecmp(tempCat.nom, c.nom) == 0) {
            printf(COULEUR_ROUGE "[ERREUR] Une catégorie avec ce nom existe déjà.\n" COULEUR_RESET);
            fclose(f);
            return 0;
        }
    }

    fseek(f, 0, SEEK_END);
    fwrite(&c, sizeof(categorie), 1, f);
    fclose(f);

    printf(COULEUR_VERT "[✓] Catégorie '%s' ajoutée avec l'ID %d.\n" COULEUR_RESET, c.nom, c.id);
    return 1;
}

void afficherCategories() {
    FILE *f = fopen(FICHIER_CATEGORIES, "rb");
    if (!f) {
        printf(COULEUR_JAUNE "[INFO] Aucune catégorie enregistrée.\n" COULEUR_RESET);
        return;
    }
    categorie c;
    int count = 0;
    printf("\n=== Liste des catégories ===\n");
    printf("%-5s %-30s\n", "ID", "Nom");
    while (fread(&c, sizeof(categorie), 1, f) == 1) {
        printf("%-5d %-30s\n", c.id, c.nom);
        count++;
    }
    if (count == 0) printf("Aucune catégorie trouvée.\n");
    fclose(f);
}

int supprimerCategorie(int id) {
    if (!confirmation("Êtes-vous sûr de vouloir supprimer cette catégorie ? (O/N) ")) return 0;

    FILE *f = fopen(FICHIER_CATEGORIES, "rb");
    FILE *temp = fopen("temp_cats.dat", "wb");
    if (!f || !temp) {
        printf(COULEUR_ROUGE "[ERREUR] Fichier introuvable.\n" COULEUR_RESET);
        if (f) fclose(f);
        if (temp) fclose(temp);
        return 0;
    }

    categorie c;
    int trouve = 0;
    while (fread(&c, sizeof(categorie), 1, f)) {
        if (c.id == id) {
            trouve = 1;
            continue;
        }
        fwrite(&c, sizeof(categorie), 1, temp);
    }
    fclose(f);
    fclose(temp);

    if (!trouve) {
        printf(COULEUR_ROUGE "[ERREUR] Catégorie non trouvée.\n" COULEUR_RESET);
        remove("temp_cats.dat");
        return 0;
    }

    if (remove(FICHIER_CATEGORIES) != 0 || rename("temp_cats.dat", FICHIER_CATEGORIES) != 0) {
        printf(COULEUR_ROUGE "[ERREUR] Erreur lors de la suppression du fichier.\n" COULEUR_RESET);
        return 0;
    }

    printf(COULEUR_VERT "[✓] Catégorie supprimée.\n" COULEUR_RESET);
    return 1;
}

int chercherCategorieParId(int id, categorie *c) {
    FILE *f = fopen(FICHIER_CATEGORIES, "rb");
    if (!f) return 0;
    while (fread(c, sizeof(categorie), 1, f)) {
        if (c->id == id) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int chercherCategorieParNom(const char *nom, categorie *c) {
    FILE *f = fopen(FICHIER_CATEGORIES, "rb");
    if (!f) return 0;
    while (fread(c, sizeof(categorie), 1, f)) {
        if (strcasecmp(c->nom, nom) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int modifierCategorie(int id) {
    FILE *f = fopen(FICHIER_CATEGORIES, "r+b");
    if (!f) {
        printf(COULEUR_ROUGE "[ERREUR] Fichier introuvable.\n" COULEUR_RESET);
        return 0;
    }
    categorie c;
    int trouve = 0;
    while (fread(&c, sizeof(categorie), 1, f)) {
        if (c.id == id) {
            trouve = 1;
            printf("Nom actuel : %s\n", c.nom);
            printf("Nouveau nom : ");
            if (fgets(c.nom, sizeof(c.nom), stdin) == NULL) {
                printf(COULEUR_ROUGE "[ERREUR] Lecture impossible.\n" COULEUR_RESET);
                fclose(f);
                return 0;
            }
            c.nom[strcspn(c.nom, "\n")] = '\0';

            categorie tempCat;
            rewind(f);
            while (fread(&tempCat, sizeof(categorie), 1, f)) {
                if (tempCat.id != id && strcasecmp(tempCat.nom, c.nom) == 0) {
                    printf(COULEUR_ROUGE "[ERREUR] Une catégorie avec ce nom existe déjà.\n" COULEUR_RESET);
                    fclose(f);
                    return 0;
                }
            }

            fseek(f, - (long)sizeof(categorie), SEEK_CUR);
            fwrite(&c, sizeof(categorie), 1, f);
            printf(COULEUR_VERT "[✓] Catégorie modifiée.\n" COULEUR_RESET);
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    printf(COULEUR_ROUGE "[ERREUR] Catégorie non trouvée.\n" COULEUR_RESET);
    return 0;
}

void menuGestionCategories() {
    int choix;
    int id;

    do {
        system("clear||cls");
        printf(COULEUR_BLEU "═══════════════════════════════\n" COULEUR_RESET);
        printf(COULEUR_BLEU "     GESTION DES CATEGORIES    \n" COULEUR_RESET);
        printf(COULEUR_BLEU "═══════════════════════════════\n" COULEUR_RESET);
        printf("1. Ajouter une catégorie\n");
        printf("2. Modifier une catégorie\n");
        printf("3. Supprimer une catégorie\n");
        printf("4. Afficher les catégories\n");
        printf("5. Retour\n");
        printf("Choix : ");

        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            choix = 0;
        }
        getchar();

        switch(choix) {
            case 1:
                ajouterCategorie();
                break;
            case 2:
                printf("ID de la catégorie à modifier : ");
                if (scanf("%d", &id) == 1) {
                    getchar();
                    modifierCategorie(id);
                } else {
                    printf(COULEUR_ROUGE "[ERREUR] ID invalide.\n" COULEUR_RESET);
                    while(getchar() != '\n');
                }
                break;
            case 3:
                printf("ID de la catégorie à supprimer : ");
                if (scanf("%d", &id) == 1) {
                    getchar();
                    supprimerCategorie(id);
                } else {
                    printf(COULEUR_ROUGE "[ERREUR] ID invalide.\n" COULEUR_RESET);
                    while(getchar() != '\n');
                }
                break;
            case 4:
                afficherCategories();
                break;
            case 5:
                printf("Retour au menu précédent.\n");
                break;
            default:
                printf(COULEUR_ROUGE "[ERREUR] Choix invalide.\n" COULEUR_RESET);
                break;
        }
        pauseEtNettoie();
    } while (choix != 5);
}
