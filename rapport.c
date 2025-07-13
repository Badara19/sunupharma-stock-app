#include <stdio.h>
#include "rapport.h"

void verifFichier(FILE *fc){
    if(fc == NULL){
        printf("Le fichier n'est pas creer !");
        exit(1);
    }
}
void creerFichier(FILE *fc){
    char ETAT[30];
    time_t maintenant = time(NULL);
    struct tm *date = localtime(&maintenant);
    strftime(ETAT, sizeof(ETAT), "ETAT_%Y%m%d.txt", date);
    fc = fopen(ETAT,"a");
    verifFichier(fc);
    fclose(fc);
    printf("Fichier creer !");
}
void venteJour(int *somVentejour){
    *somVentejour = 0;
    mettreAJourStock(Produit stock[], int nbProduitsStock, Vente vente);
    *somVentejour += (quantiteVendue * vente.produits[i].prix);
    printf("Le total des ventes du jour est de %d \n",somVentejour);
    creerFichier(fc);
}
void nbrMedVendu(int *somNbrMed){
    *somNbrMed = 0;
    mettreAJourStock(Produit stock[], int nbProduitsStock, Vente vente);
    *somNbrMed += quantiteVendue;
    printf("Le nombre de medicaments vendus est de %d \n",somNbrMed);
    creerFichier(fc);
}
void alerteStock(){
    ajoutproduit();
    mettreAJourStock(Produit stock[], int nbProduitsStock, Vente vente);
    int seuil = nbProduitsStock/2;
    int quantiteRestant = nbProduitsStock - quantiteVendue;
    if(quantiteRestant < seuil){
        printf("Alerte %s est bientot en rupture, il reste %d en stock!\n",p[i].designation,quantiteRestant);
    }
    creerFichier(fc);
}

