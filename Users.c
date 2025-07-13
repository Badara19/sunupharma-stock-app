#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Users.h"

#define FICHIER_UTILISATEUR "data/USERS.dat"

int loginvalide(char login[]) {
    if (strlen(login) != 5) {
        return 0;
    }
    for (int i = 0; i < 5; i++) {
        if (!isupper(login[i])) {
            return 0;
        }
    }
    return 1;
}

int createuser() {
    FILE *f = fopen(FICHIER_UTILISATEUR, "ab+");
    if(!f) {
        return 0;
    }
    user u;

    puts("***** CREATION D'UN COMPTE D'UTILISATEUR *****");

    do {
        printf("Login (5 lettres majuscules) : ");
        scanf("%5s",u.login);
        if(!loginvalide(u.login)) {
            puts("Login invalide !!");
        }
    }while(!loginvalide(u.login));

    printf("Prenom : ");
    scanf("%s",u.prenom);
    printf("Nom : ");
    scanf("%s",u.nom);
    printf("Telephone : ");
    scanf("%s",u.telephone);

    do {
        printf("Role (admin / pharmacien) : ");
        scanf("%s",u.role);
    } while(strcmp(u.role, "admin") != 0 && strcmp(u.role, "pharmacien") != 0);

    strcpy(u.password, "pharma123");
    u.actif = 1;
    u.firstlogin = 1;

    fwrite(&u, sizeof(user), 1, f);
    fclose(f);

    printf("Compte %s cree avec success !!",u.login);
    return 1;
}

int connexion(user *con) {
    char login[6], mdp[20];
    int trouve = 0;

    puts("***** CONNEXION *****");

    printf("Login : ");
    scanf("%5s",login);
    printf("Mot de passe : ");
    scanf("%s",mdp);

    FILE *f = fopen(FICHIER_UTILISATEUR, "rb");
    if(!f) {
        return 0;
    }
    user u;
    while(fread(&u, sizeof(user),1, f)) {
        if(strcmp(u.login, login) == 0 && strcmp(u.password, mdp) == 0) {
            if(u.actif == 0) {
                puts("Compte bloque !!");
                fclose(f);
                return 0;
            }
            *con = u;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    puts("Indentifiants incorrects !!");
    return 0;
}

void updatepassword(user * u) {
    char nouveau[20], confirme[20];

    puts("***** CHANGEMENT DE MOT DE PASSE *****");

    do {
        printf("Nouveau mot de passe : ");
        scanf("%s",nouveau);
        printf("Confirmer : ");
        scanf("%s",confirme);
        if(strcmp(nouveau, confirme) != 0) {
            puts("Les mots de passe ne correspondent pas !!");
        }
    }while(strcmp(nouveau, confirme) != 0);

    strcpy(u->password, nouveau);
    u->firstlogin = 0;

    FILE *f = fopen(FICHIER_UTILISATEUR, "rb+");
    if(!f) {
        return 0;
    }

    user tmp;
    while(fread(&tmp, sizeof(user), 1, f)) {
        if(strcmp(tmp.login, u->login) == 0) {
            fseek(f, -sizeof(user), SEEK_CUR);
            fwrite(u, sizeof(user), 1, f);
            break;
        }
    }
    fclose(f);
    puts("Mot de passe modifie avec success");
}

void bloqueuser(char login[6]) {
    FILE * f = fopen(FICHIER_UTILISATEUR, "rb+");
    if(!f) {
        puts("Impossible d'ouvrir le fichier");
        return;
    }

    user u;
    int trouve = 0;
    while(fread(&u, sizeof(user), 1, f)) {
        if(strcmp(u.login, login) == 0) {
            u.actif = 0;
            fseek(f, -sizeof(user), SEEK_CUR);
            fwrite(&u, sizeof(user), 1, f);
            trouve = 1;
            puts("Utilisateur bloque avec success !!");
            break;
        }
    }
    if(!trouve) {
        puts("Utilisateur introuvable !!");
    }
    fclose(f);
}

void debloqueuser(char login[6]) {
    FILE * f = fopen(FICHIER_UTILISATEUR, "rb+");
    if(!f) {
        puts("Impossible d'ouvrir le fichier");
        return;
    }
    user u;
    int trouve = 0;
    while(fread(&u, sizeof(user), 1, f)) {
        if(strcmp(u.login, login) == 0) {
            u.actif = 1;
            fseek(f, -sizeof(user), SEEK_CUR);
            fwrite(&u, sizeof(user), 1, f);
            trouve = 1;
            puts("Utilisateur debloque avec success !!");
            break;
        }
    }
    if(!trouve) {
        puts("Utilisateur introuvable !!");
    }
    fclose(f);
}

void afficheuser(user u) {
    char statut[10];
    if(u.actif == 1) {
        strcpy(statut, "Actif");
    }else {
        strcpy(statut, "Bloque");
    }
    printf("Login : %s\n",u.login);
    printf("Nom : %s %s\n",u.prenom,u.nom);
    printf("Telephone : %s\n",u.telephone);
    printf("Role : %s\n",u.role);
    printf("Statut : %s",statut);
}

void listeuser() {
    FILE * f = fopen(FICHIER_UTILISATEUR, "rb");
    if(!f) {
        puts("Impossible d'ouvrir le fichier !!");
        return;
    }

    user u;
    int count = 0;
    puts("***** LISTE DES UTILISATEURS *****\n");
    while (fread(&u, sizeof(user), 1, f)) {
        count++;
        afficheuser(u);
        printf("------------------------------\n");
    }

    if (count == 0) {
        puts("Aucun utilisateur trouvé !!");
    }

    fclose(f);
}
