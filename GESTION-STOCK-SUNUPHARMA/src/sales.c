#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "colors.h"
#include "users.h"
#include "products.h"
#include "categories.h"
#include "sales.h"
#include "rapports.h"

void menuGestionVentes() {
    pauseEtNettoie();
    int choix;
    do {
        puts("");
        color_title("\n========== MENU GESTION DES VENTES ==========\n");
        puts("");
        printf("1. Enregistrer une nouvelle vente\n");
        printf("2. Afficher les ventes du jour\n");
        printf("3. Afficher toutes les ventes\n");
        printf("0. Retour\n");
        printf("=============================================\n");

        choix = saisirEntierAvecMessage("Entrez votre choix :", 0, 3);

        switch (choix) {
            case 1: {
                puts("");
                color_header("********** ENREGISTREMENT DE VENTE **********\n");
                puts("");
                int idUtilisateur, idProduit, quantite;
                idUtilisateur = saisirEntierAvecMessage("ID de l'utilisateur :", 1, 1000);
                idProduit = saisirEntierAvecMessage("ID du produit :", 1, 1000);
                quantite = saisirEntierAvecMessage("Quantite :", 1, 1000);

                if (enregistrerVente(idUtilisateur, idProduit, quantite)) {
                    printf("Vente enregistree avec succes.\n");
                } else {
                    printf("Echec de l'enregistrement de la vente.\n");
                }
                break;
            }

            case 2:
                afficherVentesDuJour();
                break;

            case 3:
                afficherToutesVentes();
                break;

            case 0:
                printf("Retour au menu precedent.\n");
                break;

            default:
                printf("Choix invalide.\n");
        }

    } while (choix != 0);
}

int enregistrerVente(int idUser, int idProduit, int quantite) {
    FILE *f = fopen(FICHIER_VENTES, "ab");
    if (!f) {
        printf("Erreur lors de l'ouverture du fichier de ventes.\n");
        return 0;
    }

    sale s;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(s.date, MAX_DATE, "%Y-%m-%d", t);
    strftime(s.numero_vente, MAX_NUM_VENTE, "%Y%m%d", t);

    s.idProduit = idProduit;
    s.idUser = idUser;
    s.quantite = quantite;

    float prix = rechercherPrixProduit(idProduit);
    s.total_vente = prix * quantite;

    fwrite(&s, sizeof(sale), 1, f);
    fclose(f);

    printf("Vente enregistre avec succes !\n");
    return 1;
}

float chiffreAffairesDuJour() {
    puts("");
    color_header("********** CHIFFRE D AFFAIRES DU JOUR **********\n");
    puts("");
    FILE *f = fopen(FICHIER_VENTES, "rb");
    if (!f) return 0.0f;

    sale s;
    float total = 0.0f;
    char dateToday[9];
    time_t now = time(NULL);
    strftime(dateToday, sizeof(dateToday), "%Y%m%d", localtime(&now));

    while (fread(&s, sizeof(sale), 1, f)) {
        if (strncmp(s.numero_vente, dateToday, 8) == 0) {
            total += s.total_vente;
        }
    }

    fclose(f);
    return total;
}

void afficherVentesDuJour() {
    puts("");
    color_header("********** VENTES DU JOUR **********\n");
    puts("");
    FILE *f = fopen(FICHIER_VENTES, "rb");
    if (!f) return;

    sale s;
    char dateToday[9];
    time_t now = time(NULL);
    strftime(dateToday, sizeof(dateToday), "%Y%m%d", localtime(&now));

    printf("\n--- Ventes du jour ---\n");
    while (fread(&s, sizeof(sale), 1, f)) {
        if (strncmp(s.numero_vente, dateToday, 8) == 0) {
            printf("Vente: %s | Produit: %d | Quantit\x82: %d | Total: %.2f FCFA\n", s.numero_vente, s.idProduit, s.quantite, s.total_vente);
        }
    }
    fclose(f);
}

void afficherToutesVentes() {
    puts("");
    color_header("********** AFFICHAGE DE TOUS LES VENTES **********\n");
    puts("");
    FILE *f = fopen(FICHIER_VENTES, "rb");
    if (!f) return;

    sale s;
    printf("\n--- Toutes les ventes ---\n");
    while (fread(&s, sizeof(sale), 1, f)) {
        printf("%s | Produit: %d | Quantit\x82: %d | Date: %s | Total: %.2f FCFA\n",
               s.numero_vente, s.idProduit, s.quantite, s.date, s.total_vente);
    }
    fclose(f);
}
