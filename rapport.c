#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rapport.h"

     // Vérifie que le fichier est bien ouvert
void verifFichier(FILE *fc) {
    if (fc == NULL) {
        printf("Erreur : fichier introuvable ou non cree.\n");
        exit(1);
    }
}

    // Crée ou ouvre le fichier rapport du jour en mode ajout
FILE *ouvrirFichierRapport() {
    char nomFichier[40];
    time_t tmp = time(NULL);
    struct tm *date = localtime(&tmp);
    strftime(nomFichier, sizeof(nomFichier), "ETAT_%Y%m%d.txt", date);

    FILE *f = fopen(nomFichier, "a");
    verifFichier(f);
    return f;
}

    // Calcule le total des ventes du jour et l'affiche dans le rapport
void venteJour() {
    FILE *factures = fopen("FACTURES.dat", "rb");
    verifFichier(factures);

    struct Vente vente;
    float totalJour = 0.0f;

    time_t tmp = time(NULL);
    struct tm *dateTmp = localtime(&tmp);
    char dateToday[10];
    strftime(dateToday, sizeof(dateToday), "%Y%m%d", dateTmp);

    while (fread(&vente, sizeof(struct Vente), 1, factures) == 1) {
        char dateVente[9] = {0};
        strncpy(dateVente, vente.numero_vente, 8);

        if (strcmp(dateVente, dateToday) == 0) {
            totalJour += vente.total_vente;
        }
    }
    fclose(factures);

    FILE *rapport = ouvrirFichierRapport();
    fprintf(rapport, "Total des ventes du jour : %.3f FCFA\n", totalJour);
    fclose(rapport);

    printf("Total des ventes du jour : %.3f FCFA\n", totalJour);
}

    // Calcule le nombre total de médicaments vendus ce jour et l'écrit dans le rapport
void nbrMedVendu() {
    FILE *factures = fopen("FACTURES.dat", "rb");
    verifFichier(factures);

    struct Vente vente;
    int totalMed = 0;

    time_t now = time(NULL);
    struct tm *dateNow = localtime(&now);
    char dateToday[9];
    strftime(dateToday, sizeof(dateToday), "%Y%m%d", dateNow);

    while (fread(&vente, sizeof(struct Vente), 1, factures) == 1) {
        char dateVente[9] = {0};
        strncpy(dateVente, vente.numero_vente, 8);

        if (strcmp(dateVente, dateToday) == 0) {
            for (int i = 0; i < vente.nb_produits; i++) {
                totalMed += vente.produits[i].quantite;
            }
        }
    }
    fclose(factures);

    FILE *rapport = ouvrirFichierRapport();
    fprintf(rapport, "Nombre total de medicaments vendus aujourd'hui : %d\n", totalMed);
    fclose(rapport);

    printf("Nombre total de medicaments vendus aujourd'hui : %d\n", totalMed);
}

    // Parcourt la liste des produits et affiche une alerte si stock est faible
void alerteStock() {
    FILE *produits = fopen("PRODUCTS.dat", "rb");
    verifFichier(produits);

    FILE *rapport = ouvrirFichierRapport();

    struct produits p;
    const int seuil = 10; // seuil arbitraire pour alerte

    while (fread(&p, sizeof(struct produits), 1, produits) == 1) {
        if (p.quantiteStock < seuil) {
            fprintf(rapport, "ALERTE: %s est bientot en rupture, il reste %d en stock !\n", p.designation, p.quantiteStock);
            printf("ALERTE: %s est bientot en rupture, il reste %d en stock !\n", p.designation, p.quantiteStock);
        }
    }

    fclose(produits);
    fclose(rapport);
}

    // Affichage du Rapport
void rapport() {
    printf("=== LE RAPPORT journalier ===\n");
    venteJour();
    nbrMedVendu();
    alerteStock();
}
