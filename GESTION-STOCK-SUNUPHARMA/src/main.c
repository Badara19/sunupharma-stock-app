#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "utils.h"
#include "colors.h"
#include "users.h"
#include "products.h"
#include "categories.h"
#include "sales.h"
#include "rapports.h"

int menu_admin();
int menu_pharmacien();

int main() {
    user u;
    int choixRole, choixAction;
    char roleChoisi[15];
    puts("");
    color_header("********** BIENVENUE DANS LE SYSTEME SUNUPHARMA **********\n");
    puts("");

    color_title("Vous etes :\n");
    printf("1. Administrateur\n");
    printf("2. Pharmacien\n");
    choixRole = saisirEntierAvecMessage("Choix : ", 1, 2);
    strcpy(roleChoisi, (choixRole == 1) ? "admin" : "pharmacien");

    color_title("\nQue souhaitez-vous faire ?\n");
    printf("1. Creer un compte %s\n", roleChoisi);
    printf("2. Se connecter a un compte existant\n");
    choixAction = saisirEntierAvecMessage("Choix : ", 1, 2);

    if (choixAction == 1) {
        int success = 0;
        do {
            success = createUserAvecRole(roleChoisi);
            if (!success) {
                color_error("Echec de creation du compte. Reessayer.\n");
            }
        } while (!success);
    }

    while (!connexion(&u)) {
        color_error("Echec de l'authentification ou rôle incorrect. Reessayer.\n");
    }

    if (strcmp(u.role, roleChoisi) != 0) {
        color_error("Rôle incorrect.\n");
    }

    if (!u.actif) {
        color_error("Ce compte est desactive.\n");
        return 1;
    }

    if (u.firstlogin) {
        color_title("Premiere connexion. Veuillez changer le mot de passe.\n");
        updatePassword(&u);
    }

    color_success("Connexion reussie !\n");
    pauseEtNettoie();

    if (strcmp(u.role, "admin") == 0) {
        menu_admin();
    } else {
        menu_pharmacien();
    }

    return 0;
}

int menu_admin() {
    int choix;
    do {
        pauseEtNettoie();
        puts("");
        color_title("********** MENU ADMINISTRATEUR **********\n");
        puts("");
        printf("1. Gestion Medicaments\n");
        printf("2. Gestion Categories\n");
        printf("3. Gestion Utilisateurs\n");
        printf("4. Gestion Ventes\n");
        printf("5. Generation Rapports\n");
        printf("0. Quitter\n");

        choix = saisirEntierAvecMessage("Votre choix : ", 0, 5);

        switch (choix) {
            case 1: menuGestionProduits(); break;
            case 2: menuGestionCategories(); break;
            case 3: menuGestionUtilisateurs(); break;
            case 4: menuGestionVentes(); break;
            case 5: menuRapport(); break;
            case 0: color_success("Deconnexion...\n"); break;
        }
        pauseEtNettoie();
    } while (choix != 0);
    return 0;
}

int menu_pharmacien() {
    user u;
    produit m;
    int choix;
    do {
            pauseEtNettoie();
        puts("");
        color_title("********** MENU PHARMACIEN **********\n");
        puts("");
        printf("1. Enregistrer une vente\n");
        printf("2. Consulter le stock\n");
        printf("3. Etat des ventes du jour\n");
        printf("0. Quitter\n");

        choix = saisirEntierAvecMessage("Votre choix : ", 0, 3);

        switch (choix) {
            case 1: enregistrerVente(u.id, m.id, m.quantiteStock); break;
            case 2: afficherTousMedicaments(); break;
            case 3:
                afficherVentesDuJour();
                printf("Chiffre d'affaires du jour : %.2f\n", chiffreAffairesDuJour());
                break;
            case 0: color_success("Deconnexion...\n"); break;
        }
        pauseEtNettoie();
    } while (choix != 0);
    return 0;
}
