#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include "colors.h"
#include "users.h"
#include "products.h"
#include "categories.h"
#include "sales.h"
#include "rapports.h"

#define FICHIER_PRODUITS "../datas/produits.dat"

bool estBissextile(int annee) {
    return (annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0);
}

bool estDateValide(int jour, int mois, int annee) {
    if (annee < 2023 || mois < 1 || mois > 12 || jour < 1)
        return false;

    int joursParMois[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (mois == 2 && estBissextile(annee))
        joursParMois[1] = 29;

    return jour <= joursParMois[mois - 1];
}

int genererIdMedicament() {
    FILE *f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) return 1;
    produit m;
    int maxId = 0;
    while (fread(&m, sizeof(produit), 1, f)) {
        if (m.id > maxId) maxId = m.id;
    }
    fclose(f);
    return maxId + 1;
}

void afficherMedicament(produit m) {
    pauseEtNettoie();
    puts("");
    color_header("********** AFFICHAGE DE MEDICAMENT **********\n");
    puts("");
    char date[11];
    snprintf(date, sizeof(date), "%02d/%02d/%04d", m.datePeremption.jour, m.datePeremption.mois, m.datePeremption.annee);
    printf("ID : %-4d\n", m.id);
    printf("NOM : %-20s\n", m.nom);
    printf("CATEGORIE : %-15s\n", getNomCategorie(m.idCategorie));
    printf("QUANTITE : %-8d\n", m.quantiteStock);
    printf("PRIX : %7.2f\n", m.prixUnitaire);
    printf("SEUIL : %-8d\n", m.seuil);
    printf("DATE : %-10s\n", date);
}


int ajouterMedicament() {
    pauseEtNettoie();
    puts("");
    color_header("********** AJOUT DE MEDICAMENT **********\n");
    puts("");
    produit m;
    m.id = genererIdMedicament();

    printf("Nom du medicament : ");
    lireChaine(m.nom, MAX_NOM);

    afficherToutesCategories();
    printf("ID categorie : ");
    m.idCategorie = saisirEntier(1, 1000);

    if (!existeCategorie(m.idCategorie)) {
        color_error("Categorie inexistante\n");
        return 0;
    }

    printf("Quantite en stock : ");
    m.quantiteStock = saisirEntier(0, 100000);

    printf("Seuil d'alerte : ");
    m.seuil = saisirEntier(1, 100000);

    int jour, mois, annee;
    do {
        printf("Date de peremption (jj mm aaaa)\n");
        printf("Jour : ");
        jour = saisirEntier(1, 31);
        mois = saisirEntier(1, 12);
        annee = saisirEntier(2025, 2100);

        if (!estDateValide(jour, mois, annee)) {
            printf("Date invalide ! Veuillez reessayer.\n");
        }
    } while (!estDateValide(jour, mois, annee));

    m.datePeremption.jour = jour;
    m.datePeremption.mois = mois;
    m.datePeremption.annee = annee;

    printf("Prix unitaire : ");
    scanf("%f", &m.prixUnitaire);
    viderBuffer();

    FILE *f = fopen(FICHIER_PRODUITS, "ab");
    if (!f) {
        color_error("Erreur ouverture fichier\n");
        return 0;
    }
    fwrite(&m, sizeof(produit), 1, f);
    fclose(f);
    color_success("\nMedicament ajoute avec succes\n");
    return 1;
}

void afficherTousMedicaments() {
    pauseEtNettoie();
    puts("");
    color_header("********** AFFICHAGE DE TOUS LES MEDICAMENTS **********\n");
    puts("");
    FILE *f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) {
        color_error("Erreur ouverture fichier\n");
        return;
    }
    produit m;
    while (fread(&m, sizeof(produit), 1, f)) {
        afficherMedicament(m);
    }
    fclose(f);
}

produit* rechercherMedicamentParId(int id) {
    pauseEtNettoie();
    puts("");
    color_header("********** RECHERCHE DE MEDICAMENT **********\n");
    puts("");
    FILE *f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) return NULL;
    produit *m = malloc(sizeof(produit));
    while (fread(m, sizeof(produit), 1, f)) {
        if (m->id == id) {
            fclose(f);
            return m;
        }
    }
    fclose(f);
    free(m);
    return NULL;
}

int existeMedicament(int id) {
    produit *m = rechercherMedicamentParId(id);
    if (m) {
        free(m);
        return 1;
    }
    return 0;
}

void modifierMedicament(int id) {
    pauseEtNettoie();
    puts("");
    color_header("********** MODIFICATION DE MEDICAMENT **********\n");
    puts("");
    FILE *f = fopen(FICHIER_PRODUITS, "r+b");
    if (!f) {
        color_error("Erreur ouverture fichier\n");
        return;
    }
    produit m;
    while (fread(&m, sizeof(produit), 1, f)) {
        if (m.id == id) {
            color_title("Modification du medicament ID");

            printf("\nNom actuel: %s\nNouveau nom (vide pour garder): ", m.nom);
            char buffer[MAX_NOM];
            lireChaine(buffer, MAX_NOM);
            if (strlen(buffer) > 0) strcpy(m.nom, buffer);

            afficherToutesCategories();
            printf("Categorie actuelle: %s\nNouvelle categorie (0 pour garder): \n", getNomCategorie(m.idCategorie));
            int newCat = saisirEntier(0, 1000);
            if (newCat != 0 && existeCategorie(newCat)) {
                m.idCategorie = newCat;
            }

            printf("Quantite actuelle: %d\nNouvelle quantite (-1 pour garder): \n", m.quantiteStock);
            int qte = saisirEntier(-1, 100000);
            if (qte != -1) m.quantiteStock = qte;

            printf("Seuil actuel: %d\nNouveau seuil (-1 pour garder): \n", m.seuil);
            int seuil = saisirEntier(-1, 100000);
            if (seuil != -1) m.seuil = seuil;

            printf("Date de peremption actuelle: %02d/%02d/%04d\n", m.datePeremption.jour, m.datePeremption.mois, m.datePeremption.annee);
            printf("Nouvelle date (jj mm aaaa, 0 0 0 pour garder): \n");
            int j, mo, a;
            scanf("%d %d %d", &j, &mo, &a);
            viderBuffer();
            if (!(j == 0 && mo == 0 && a == 0)) {
                if (j >= 1 && j <= 31 && mo >= 1 && mo <= 12 && a >= 2023 && a <= 2100) {
                    m.datePeremption.jour = j;
                    m.datePeremption.mois = mo;
                    m.datePeremption.annee = a;
                }
            }

            printf("Prix actuel: %.2f\nNouveau prix (-1 pour garder): \n", m.prixUnitaire);
            float prix;
            scanf("%f", &prix);
            viderBuffer();
            if (prix >= 0) m.prixUnitaire = prix;

            fseek(f, - (long)sizeof(produit), SEEK_CUR);
            fwrite(&m, sizeof(produit), 1, f);
            fflush(f);
            fclose(f);
            color_success("\nMedicament modifie\n");
            return;
        }
    }
    fclose(f);
    color_error("Medicament non trouve\n");
}

void afficherMedicamentsParCategorie(int idCategorie) {
    pauseEtNettoie();
    puts("");
    color_header("********** AFFICHAGE DE MEDICAMENT PAR CATEGORIE **********\n");
    puts("");
    FILE *f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) {
        color_error("Erreur ouverture fichier\n");
        return;
    }
    produit m;
    while (fread(&m, sizeof(produit), 1, f)) {
        if (m.idCategorie == idCategorie) {
            afficherMedicament(m);
        }
    }
    fclose(f);
}

void menuGestionProduits() {
    int choix;
    while (1) {
        pauseEtNettoie();
        puts("");
        color_title("\n====== GESTION DES PRODUITS ======\n");
        puts("");
        printf("1. Ajouter un medicament\n");
        printf("2. Afficher tous les medicaments\n");
        printf("3. Modifier un medicament\n");
        printf("4. Supprimer un medicament\n");
        printf("5. Afficher medicaments par categorie\n");
        printf("6. Retour\n");
        printf("Choix : ");
        choix = saisirEntier(1, 6);

        if (choix == 1) {
            ajouterMedicament();
            pauseEtNettoie();
        } else if (choix == 2) {
            afficherTousMedicaments();
            pauseEtNettoie();
        } else if (choix == 3) {
            printf("ID du medicament a modifier : ");
            int id = saisirEntier(1, 100000);
            modifierMedicament(id);
            pauseEtNettoie();
        } else if (choix == 4) {
            printf("ID du medicament a supprimer : ");
            int id = saisirEntier(1, 100000);
            supprimerMedicament(id);
            pauseEtNettoie();
        } else if (choix == 5) {
            afficherToutesCategories();
            printf("ID categorie : ");
            int idCat = saisirEntier(1, 1000);
            afficherMedicamentsParCategorie(idCat);
            pauseEtNettoie();
        } else {
            break;
        }
    }
}

const char* getNomMedicament(int idMedicament) {
    static char nom[100];
    FILE *f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) return "Inconnu";

    produit m;
    while (fread(&m, sizeof(produit), 1, f)) {
        if (m.id == idMedicament) {
            strncpy(nom, m.nom, sizeof(nom));
            fclose(f);
            return nom;
        }
    }
    fclose(f);
    return "Inconnu";
}

void afficherProduits() {
    puts("");
    color_header("********** AFFICHAGE DES PRODUITS **********\n");
    puts("");
    FILE* f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) {
        color_error("Aucun produit trouve.\n");
        return;
    }

    produit p;
    int vide = 1;

    printf("\n%-5s %-20s %-10s %-10s %-10s %-20s %-10s\n", "ID", "Nom", "Prix", "Stock", "Seuil", "Categorie", "Peremption");

    while (fread(&p, sizeof(produit), 1, f)) {
        char* nomCat = getNomCategorie(p.idCategorie);
        printf("%-5d %-20s %-10.2f %-10d %-10d %-20s %02d/%02d/%04d\n",
               p.id, p.nom, p.prixUnitaire, p.quantiteStock, p.seuil,
               nomCat ? nomCat : "Inconnue",
               p.datePeremption.jour, p.datePeremption.mois, p.datePeremption.annee);
        vide = 0;
    }

    fclose(f);

    if (vide) {
        printf("Aucun produit enregistre.\n");
    }
}

int supprimerMedicament(int id) {
    puts("");
    color_header("********** SUPPRESSION DE MEDICAMENT **********\n");
    puts("");
    FILE *f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) {
        color_error("Erreur ouverture fichier\n");
        return 0;
    }

    FILE *temp = fopen("datas/temp.dat", "wb");
    if (!temp) {
        fclose(f);
        color_error("Erreur creation fichier temporaire\n");
        return 0;
    }

    produit m;
    int trouve = 0;
    while (fread(&m, sizeof(produit), 1, f)) {
        if (m.id != id) {
            fwrite(&m, sizeof(produit), 1, temp);
        } else {
            trouve = 1;
        }
    }

    fclose(f);
    fclose(temp);

    if (trouve) {
        remove(FICHIER_PRODUITS);
        rename("datas/temp.dat", FICHIER_PRODUITS);
        color_success("Medicament supprime\n");
        return 1;
    } else {
        remove("datas/temp.dat");
        color_error("Medicament non trouve\n");
        return 0;
    }
}

float rechercherPrixProduit(int idProduit) {
    puts("");
    color_header("********** RECHERCHE DE PRODUIT **********\n");
    puts("");
    FILE *f = fopen(FICHIER_PRODUITS, "rb");
    if (!f) {
        perror("Erreur ouverture fichier produits");
        return -1;
    }

    produit p;
    while (fread(&p, sizeof(produit), 1, f)) {
        if (p.id == idProduit) {
            fclose(f);
            return p.prixUnitaire;
        }
    }

    fclose(f);
    return -1;
}
