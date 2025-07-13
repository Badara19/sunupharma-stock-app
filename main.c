#include <stdio.h>
#include <stdlib.h>
#include "Users.h"

int main() {
    int choix;
    char login[6];
    user u;

    do {
        puts("*****************************************");
        puts("       MENU UTILISATEURS SUNUPHARMA      ");
        puts("*****************************************");
        puts("1. Creer un utilisateur");
        puts("2. Connexion");
        puts("3. Lister tous les utilisateurs");
        puts("4. Bloquer un utilisateur");
        puts("5. Debloquer un utilisateur");
        puts("0. Quitter");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                createuser();
                break;
            case 2:
                if (connexion(&u)) {
                    printf("Connecte : %s\n", u.login);
                    if (u.firstlogin) updatepassword(&u);
                    afficheuser(u);
                }
                break;
            case 3:
                listeuser();
                break;
            case 4:
                printf("Login a bloquer : ");
                scanf("%s", login);
                bloqueuser(login);
                break;
            case 5:
                printf("Login a debloquer : ");
                scanf("%s", login);
                debloqueuser(login);
                break;
            case 0:
                puts("Fermeture du programme... Au revoir !");
            break;
            default:
                puts("Choix invalide !");
        }
    } while (choix != 0);

    return 0;
}
