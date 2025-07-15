#include "sales.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int i;

void initVente(Vente *vente, const char *nomPharmacien) {
    time_t maintenant = time(NULL);
    struct tm *temps = localtime(&maintenant);

    strftime(vente->numero_vente, sizeof(vente->numero_vente), "%Y%m%d%H%M%S", temps);
    strncpy(vente->login, nomPharmacien, sizeof(vente->login) - 1);
    strncpy(vente->nom_pharmacie, nomPharmacien, sizeof(vente->nom_pharmacie) - 1);
    vente->login[sizeof(vente->login) - 1] = '\0';
    vente->nb_produits = 0;
    vente->total_vente = 0.0;

    printf("Vente initialisee : numero = %s, login = %s\n", vente->numero_vente, vente->login);
}


void calculerTotalVente(Vente *vente) {
    vente->total_vente = 0.0;
    for (i = 0; i < vente->nb_produits; i++) {
        vente->total_vente += vente->produits[i].total;
    }
}

void mettreAJourStock(Produit stock[], int nbProduitsStock, Vente vente) {
    printf("\nMise a jour du stock...\n");

    for (int i = 0; i < vente.nb_produits; i++) {
        const char *codeVendu = vente.produits[i].code;
        int quantiteVendue = vente.produits[i].quantite;
        int trouve = 0;

        for (int j = 0; j < nbProduitsStock; j++) {
            if (strcmp(stock[j].code, codeVendu) == 0) {
                trouve = 1;
                printf("Produit trouve : %s | Stock actuel : %d | Quantite demandee : %d\n",
                       stock[j].code, stock[j].quantite_stock, quantiteVendue);

                if (stock[j].quantite_stock >= quantiteVendue) {
                    stock[j].quantite_stock -= quantiteVendue;
                    printf("Stock mis a jour. Nouveau stock : %d\n", stock[j].quantite_stock);
                } else {
                    printf("Stock insuffisant pour %s. Mise a jour impossible.\n", stock[j].code);
                }

                break;
            }
        }

        if (!trouve)
            printf("Produit %s non trouve dans le stock !\n", codeVendu);
    }
}

void genererFacture(Vente *vente, const char *idPharmacien) {
    printf("\n================= FACTURE =================\n");
    printf("Pharmacie      : %s\n", vente->nom_pharmacie);
    printf("ID Pharmacien  : %s\n", idPharmacien);
    printf("------------------------------------------\n");
    printf("Produit   | Prix Unitaire | Quantite | Total\n");
    printf("------------------------------------------\n");

    for (int i = 0; i < vente->nb_produits; i++) {
        printf("%-9s | %13.2f | %8d | %6.2f\n",
               vente->produits[i].code,
               vente->produits[i].prix_unitaire,
               vente->produits[i].quantite,
               vente->produits[i].total);
    }

    printf("------------------------------------------\n");
    printf("TOTAL a PAYER : %.2f FCFA\n", vente->total_vente);
    printf("==========================================\n");
}

void afficherMessage(const char *message, int estErreur) {
    if (estErreur) {
        printf("[ERREUR] %s\n", message);
    } else {
        printf("[INFO] %s\n", message);
    }
}

void afficherMenuVente() {
    printf("\n=== BIENVENUE DANS LE MENU DES GESTIONS DE VENTES ET FACTURES ===\n");
    printf("1. Initialiser une vente\n");
    printf("2. Ajouter un produit a la vente par son code\n");
    printf("3. Calculer le total de la vente\n");
    printf("4. Mettre a jour le stock\n");
    printf("5. Generer une facture\n");
    printf("6. Quitter\n");
    printf("Choix : ");
}

void ajouterProduitVente(Produit stock[], int nbProduits) {
    char codeRecherche[20];
    int trouve = 0;

    printf("Entrez le code du produit a rechercher : ");
    scanf("%s", codeRecherche);

    for (i = 0; i < nbProduits; i++) {
        if (strcmp(stock[i].code, codeRecherche) == 0) {
            printf("\n--- Produit trouve ---\n");
            printf("Code           : %s\n", stock[i].code);
            printf("Designation    : %s\n", stock[i].designation);
            printf("Prix           : %.2f FCFA\n", stock[i].prix);
            printf("Quantite Stock : %d\n", stock[i].quantite_stock);
            printf("Categorie      : %s\n", stock[i].categorie);
            printf("Date Peremption: %s\n", stock[i].date_peremption);
            trouve = 1;
            break;
        }
    }

    if (!trouve) {
        printf("Aucun produit ne correspond au code entre.\n");
    }

    printf("\nAppuyez sur Entree pour continuer...");
    getchar(); getchar();
}

void gestionVentes(Produit stock[], int nbProduits, Vente *vente, const char *login) {
    int choix;
    char codeProduit[20];
    float prixUnitaire;
    int quantite;

    do {
        system("cls");
        afficherMenuVente();
        scanf("%d", &choix);
        getchar();

        switch (choix) {
            case 1:
                initVente(vente, login);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;

            case 2:
                ajouterProduitVente(stock, nbProduits);
                break;

            case 3:
                calculerTotalVente(vente);
                printf("Total calcule : %.2f FCFA\n", vente->total_vente);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;

            case 4:
                mettreAJourStock(stock, nbProduits, *vente);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;

            case 5:
                genererFacture(vente, login);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;


            case 6:
                printf("Retour au menu principal...\n");
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;

            default:
                afficherMessage("Choix invalide.", 1);
        }
    } while (choix != 6);
}
