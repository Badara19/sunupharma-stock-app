#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "Users.h"
#include "Products.h"
#include "Categories.h"
#include "Sales.h"
#include "Utils.h"
#include "Colors.h"

int main() {
    user u;

    if (!adminExiste()) {
        printf(COULEUR_JAUNE "[INFO] Aucun administrateur trouvé. Veuillez créer un compte admin.\n" COULEUR_RESET);
        if (!createUser()) {
            printf(COULEUR_ROUGE "[ERREUR] Échec création compte administrateur.\n" COULEUR_RESET);
            return 1;
        }
    }

    while (1) {
        printf("\n=== CONNEXION ===\n");
        if (!connexion(&u)) {
            printf(COULEUR_ROUGE "[ERREUR] Échec de la connexion. Veuillez réessayer.\n" COULEUR_RESET);
            pauseEtNettoie();
            continue;
        }

        afficherResumeUser(u);

        if (u.firstlogin) {
            printf(COULEUR_JAUNE "Votre premier login, veuillez changer votre mot de passe.\n" COULEUR_RESET);
            updatePassword(&u);
        }

        if (strcasecmp(u.role, "admin") == 0) {
            menuAdmin(u);
        } else if (strcasecmp(u.role, "pharmacien") == 0) {
            menuPharmacien(u);
        } else {
            printf(COULEUR_ROUGE "[ERREUR] Rôle utilisateur inconnu : %s\n" COULEUR_RESET, u.role);
        }

        printf("\nDECONNEXION.\n");
    }

    return 0;
}
