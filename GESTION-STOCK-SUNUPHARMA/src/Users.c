#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Users.h"
#include "Utils.h"
#include "Colors.h"

#define FICHIER_UTILISATEURS "../datas/USERS.dat"

int loginValide(char login[]) {
    if (strlen(login) != 5) return 0;
    for (int i = 0; i < 5; i++) {
        if (!isupper(login[i])) return 0;
    }
    return 1;
}

int passwordValide(char pass[]) {
    return strlen(pass) >= 5;
}

int adminExiste() {
    FILE *f = fopen(FICHIER_UTILISATEURS, "rb");
    if (!f) return 0;
    user u;
    while (fread(&u, sizeof(user), 1, f)) {
        if (strcmp(u.role, "admin") == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int createUser() {
    user u;
    printf("Login (5 MAJ): ");
    scanf("%s", u.login);
    if (!loginValide(u.login)) return 0;

    strcpy(u.password, "pharma123");

    printf("Prenom : ");
    scanf("%s", u.prenom);
    printf("Nom : ");
    scanf("%s", u.nom);
    printf("Telephone : ");
    scanf("%s", u.telephone);
    printf("Role (admin/pharmacien) : ");
    scanf("%s", u.role);

    u.actif = 1;
    u.firstlogin = 1;

    FILE *f = fopen(FICHIER_UTILISATEURS, "ab");
    if (!f) return 0;
    fwrite(&u, sizeof(user), 1, f);
    fclose(f);
    return 1;
}

int connexion(user *u) {
    char login[MAX_LOGIN];
    char pass[MAX_PASS];
    printf("Login : ");
    scanf("%s", login);
    printf("Mot de passe : ");
    scanf("%s", pass);

    FILE *f = fopen(FICHIER_UTILISATEURS, "rb");
    if (!f) return 0;
    user tmp;
    while (fread(&tmp, sizeof(user), 1, f)) {
        if (strcmp(tmp.login, login) == 0 && strcmp(tmp.password, pass) == 0 && tmp.actif == 1) {
            *u = tmp;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void afficherResumeUser(user u) {
    printf("%s %s (%s) - %s\n", u.prenom, u.nom, u.login, u.role);
}

void updatePassword(user *u) {
    char newpass[MAX_PASS];
    printf("Nouveau mot de passe : ");
    scanf("%s", newpass);

    FILE *f = fopen(FICHIER_UTILISATEURS, "rb+");
    if (!f) return;
    user tmp;
    while (fread(&tmp, sizeof(user), 1, f)) {
        if (strcmp(tmp.login, u->login) == 0) {
            strcpy(tmp.password, newpass);
            tmp.firstlogin = 0;
            fseek(f, -sizeof(user), SEEK_CUR);
            fwrite(&tmp, sizeof(user), 1, f);
            break;
        }
    }
    fclose(f);
}

void bloquerUtilisateur(const char *login) {
    FILE *f = fopen(FICHIER_UTILISATEURS, "rb+");
    if (!f) return;
    user u;
    while (fread(&u, sizeof(user), 1, f)) {
        if (strcmp(u.login, login) == 0) {
            u.actif = 0;
            fseek(f, -sizeof(user), SEEK_CUR);
            fwrite(&u, sizeof(user), 1, f);
            break;
        }
    }
    fclose(f);
}

void debloquerUtilisateur(const char *login) {
    FILE *f = fopen(FICHIER_UTILISATEURS, "rb+");
    if (!f) return;
    user u;
    while (fread(&u, sizeof(user), 1, f)) {
        if (strcmp(u.login, login) == 0) {
            u.actif = 1;
            fseek(f, -sizeof(user), SEEK_CUR);
            fwrite(&u, sizeof(user), 1, f);
            break;
        }
    }
    fclose(f);
}

void supprimerUtilisateur(const char *login) {
    FILE *f = fopen(FICHIER_UTILISATEURS, "rb");
    FILE *temp = fopen("TEMP.dat", "wb");
    if (!f || !temp) return;
    user u;
    while (fread(&u, sizeof(user), 1, f)) {
        if (strcmp(u.login, login) != 0) {
            fwrite(&u, sizeof(user), 1, temp);
        }
    }
    fclose(f);
    fclose(temp);
    remove(FICHIER_UTILISATEURS);
    rename("TEMP.dat", FICHIER_UTILISATEURS);
}

void afficherTousUtilisateurs() {
    FILE *f = fopen(FICHIER_UTILISATEURS, "rb");
    if (!f) return;
    user u;
    while (fread(&u, sizeof(user), 1, f)) {
        afficherResumeUser(u);
    }
    fclose(f);
}

void menuGestionUtilisateurs() {
    int choix;
    do {
        printf("1. Creer un compte\n2. Bloquer un compte\n3. Débloquer un compte\n4. Supprimer un compte\n5. Lister les comptes\n0. Quitter\n");
        scanf("%d", &choix);
        char login[MAX_LOGIN];
        switch (choix) {
            case 1: createUser(); break;
            case 2: printf("Login du compte à bloquer : "); scanf("%s", login); bloquerUtilisateur(login); break;
            case 3: printf("Login du compte à débloquer : "); scanf("%s", login); debloquerUtilisateur(login); break;
            case 4: printf("Login du compte à supprimer : "); scanf("%s", login); supprimerUtilisateur(login); break;
            case 5: afficherTousUtilisateurs(); break;
        }
    } while (choix != 0);
}

void menuAdmin(user u) {
    printf("Bienvenue Admin %s\n", u.login);
    menuGestionUtilisateurs();
}

void menuPharmacien(user u) {
    printf("Bienvenue Pharmacien %s\n", u.login);
}
