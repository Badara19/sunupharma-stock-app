#include "rapports.h"
#include "sales.h"
#include "products.h"
#include "utils.h"
#include "colors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DOSSIER_BILLS "bills/"

void genererRapportJournalier(void) {
    char nomFichier[128];
    time_t now = time(NULL);
    struct tm *dateNow = localtime(&now);
    strftime(nomFichier, sizeof(nomFichier), DOSSIER_BILLS "../bills/ETAT_%Y%m%d.txt", dateNow);

    FILE *fVentes = fopen(FICHIER_VENTES, "rb");
    verifFichier(fVentes);

    FILE *fProduits = fopen(FICHIER_PRODUITS, "rb");
    verifFichier(fProduits);

    FILE *rapport = fopen(nomFichier, "w");
    verifFichier(rapport);

    float totalVentesJour = 0.0f;
    int totalMedVendus = 0;

    char dateToday[MAX_DATE];
    strftime(dateToday, sizeof(dateToday), "%Y-%m-%d", dateNow);

    sale vente;
    while (fread(&vente, sizeof(sale), 1, fVentes) == 1) {
        if (strcmp(vente.date, dateToday) == 0) {
            totalMedVendus += vente.quantite;
            totalVentesJour += vente.total_vente;
        }
    }

    fprintf(rapport, "Rapport journalier du %s\n", dateToday);
    fprintf(rapport, "-------------------------------------\n");
    fprintf(rapport, "Total des ventes du jour : %.2f FCFA\n", totalVentesJour);
    fprintf(rapport, "Nombre total de medicaments vendus : %d\n\n", totalMedVendus);

    fprintf(rapport, "ALERTES STOCKS CRITIQUES (quantite < seuil) :\n");

    produit p;
    int alerteTrouvee = 0;
    while (fread(&p, sizeof(produit), 1, fProduits) == 1) {
        if (p.quantiteStock < p.seuil) {
            fprintf(rapport, "- %s (stock actuel : %d, seuil critique : %d)\n", p.nom, p.quantiteStock, p.seuil);
            alerteTrouvee = 1;
        }
    }
    if (!alerteTrouvee) {
        fprintf(rapport, "Aucune alerte stock.\n");
    }

    fclose(fVentes);
    fclose(fProduits);
    fclose(rapport);

    color_success("Rapport journalier genere avec succes\n");
}

void afficherRapportJournalier(void) {
    char nomFichier[128];
    time_t now = time(NULL);
    struct tm *dateNow = localtime(&now);
    strftime(nomFichier, sizeof(nomFichier), DOSSIER_BILLS "../bills/ETAT_%Y%m%d.txt", dateNow);

    FILE *f = fopen(nomFichier, "r");
    if (!f) {
        color_error("Aucun rapport disponible pour aujourd hui.\n");
        return;
    }

    char ligne[256];
    puts("");
    color_title("\n********** CONTENU DU RAPPORT JOURNALIER **********\n");
    puts("");
    while (fgets(ligne, sizeof(ligne), f)) {
        printf("%s", ligne);
    }
    fclose(f);
}

void menuRapport(void) {
    pauseEtNettoie();
    int choix;
    do {
        puts("");
        color_header("********** MENU RAPPORTS JOURNALIERS **********");
        puts("");
        printf("1. Generer le rapport journalier complet\n");
        printf("2. Afficher le rapport journalier\n");
        printf("0. Retour au menu principal\n");

        choix = saisirEntierAvecMessage("\nEntrez votre choix [0-2] : ", 0, 2);
        printf("\n");

        switch (choix) {
            case 1:
                genererRapportJournalier();
                break;
            case 2:
                afficherRapportJournalier();
                break;
            case 0:
                color_success("Retour au menu principal...\n");
                break;
            default:
                color_error("Choix invalide.\n");
        }

    } while (choix != 0);
}
