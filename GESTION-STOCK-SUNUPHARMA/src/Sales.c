#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Sales.h"
#include "Products.h"
#include "Utils.h"
#include "Colors.h"

#define FICHIER_VENTES "../datas/SALES.dat"
#define DOSSIER_BILLS "BILLS/"

static void genererIdVente(char *buffer, size_t taille) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buffer, taille, "%04d%02d%02d%02d%02d%02d",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void enregistrerVente(Vente v) {
    FILE *f = fopen(FICHIER_VENTES, "ab");
    if (!f) return;
    fwrite(&v, sizeof(Vente), 1, f);
    fclose(f);
}

void afficherFacture(Vente v) {
    char nomFichier[256];
    snprintf(nomFichier, sizeof(nomFichier), "%sRECU_%s_%s.txt", DOSSIER_BILLS, v.idVente, v.vendeur);

    FILE *f = fopen(nomFichier, "w");
    if (!f) return;

    fprintf(f, "=== FACTURE DE VENTE ===\n");
    fprintf(f, "ID Vente     : %s\n", v.idVente);
    fprintf(f, "Pharmacien  : %s\n", v.vendeur);
    fprintf(f, "Date        : %02d/%02d/%04d\n\n", v.dateVente.jour, v.dateVente.mois, v.dateVente.annee);

    fprintf(f, "M\202dicament  : %s\n", v.designation);
    fprintf(f, "Code        : %s\n", v.codeMedicament);
    fprintf(f, "Quantit\202    : %d\n", v.quantite);
    fprintf(f, "Prix unitaire : %.2f XOF\n", v.prixUnitaire);
    fprintf(f, "Total       : %.2f XOF\n", v.total);

    fclose(f);
}

void effectuerVente(user vendeur) {
    char code[6];
    Medicament med;
    int quantite;
    Vente v;

    printf("Code m\202dicament \205 vendre : ");
    fgets(code, sizeof(code), stdin);
    code[strcspn(code, "\n")] = '\0';
    for (int i = 0; code[i]; i++) code[i] = toupper(code[i]);

    if (!chercherMedicamentParCode(code, &med)) return;

    Date auj = dateAujourdHui();

    if (!dateSuperieure(med.datePeremption, auj)) return;

    printf("Quantit\202 \205 vendre : ");
    if (scanf("%d", &quantite) != 1 || quantite <= 0) {
        while(getchar()!='\n');
        return;
    }
    while(getchar()!='\n');

    if (quantite > med.quantite) return;

    if (!miseAJourStock(med.code, quantite)) return;

    genererIdVente(v.idVente, sizeof(v.idVente));
    strcpy(v.codeMedicament, med.code);
    strcpy(v.designation, med.nom);
    v.quantite = quantite;
    v.prixUnitaire = med.prixUnitaire;
    v.total = quantite * med.prixUnitaire;
    v.dateVente = auj;
    strcpy(v.vendeur, vendeur.login);

    enregistrerVente(v);
    afficherFacture(v);
}

void afficherVentesDuJour() {
    FILE *f = fopen(FICHIER_VENTES, "rb");
    if (!f) return;

    Vente v;
    Date auj = dateAujourdHui();

    while (fread(&v, sizeof(Vente), 1, f) == 1) {
        if (v.dateVente.jour == auj.jour &&
            v.dateVente.mois == auj.mois &&
            v.dateVente.annee == auj.annee) {
            afficherFacture(v);
        }
    }
    fclose(f);
}

void afficherToutesVentes() {
    FILE *f = fopen(FICHIER_VENTES, "rb");
    if (!f) return;

    Vente v;
    while (fread(&v, sizeof(Vente), 1, f) == 1) {
        afficherFacture(v);
    }
    fclose(f);
}

float chiffreAffairesJournalier() {
    FILE *f = fopen(FICHIER_VENTES, "rb");
    if (!f) return 0.0f;

    Vente v;
    float total = 0.0f;
    Date auj = dateAujourdHui();

    while (fread(&v, sizeof(Vente), 1, f) == 1) {
        if (v.dateVente.jour == auj.jour &&
            v.dateVente.mois == auj.mois &&
            v.dateVente.annee == auj.annee) {
            total += v.total;
        }
    }
    fclose(f);
    return total;
}

void menuGestionVentes() {
    int choix;
    user vendeur;
    strcpy(vendeur.login, "PHARM");

    do {
        system("cls||clear");
        printf("1. Effectuer une vente\n");
        printf("2. Afficher ventes du jour\n");
        printf("3. Afficher toutes les ventes\n");
        printf("4. Chiffre d'affaires journalier\n");
        printf("5. Retour\n");
        printf("Choix : ");

        if (scanf("%d", &choix) != 1) {
            while(getchar()!='\n');
            choix = 0;
        }
        getchar();

        switch(choix) {
            case 1:
                effectuerVente(vendeur);
                pauseEtNettoie();
                break;
            case 2:
                afficherVentesDuJour();
                pauseEtNettoie();
                break;
            case 3:
                afficherToutesVentes();
                pauseEtNettoie();
                break;
            case 4:
                printf("CA du jour : %.2f XOF\n", chiffreAffairesJournalier());
                pauseEtNettoie();
                break;
            case 5:
                break;
            default:
                pauseEtNettoie();
                break;
        }
    } while (choix != 5);
}
