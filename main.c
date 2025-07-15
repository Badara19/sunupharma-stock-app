#include <stdio.h>
#include <stdlib.h>
#include "sales.h"

#define MAX_STOCK 100

int main() {

    // ✅ Initialisation du stock avec 2
    Produit stock[MAX_STOCK] = {
        {"P001", "Paracétamol", 150.0, 10, "Médicament", "2025-12-31"},
        {"P002", "Ibuprofène", 1075.5, 5, "Médicament", "2026-06-30"}
    };
    int nbProduits = 2;

    // ✅ Initialisation de la vente vide
    Vente vente;

    // ✅ Login ou identifiant du pharmacien
    char login[100] = "SUNUPHARMA";

    // ✅ Lancement du menu de ventes
    gestionVentes(stock, nbProduits, &vente, login);

    return 0;
}
