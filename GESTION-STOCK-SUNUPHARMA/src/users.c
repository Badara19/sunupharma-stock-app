#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "colors.h"
#include "users.h"
#include "products.h"
#include "categories.h"
#include "sales.h"
#include "rapports.h"

#define FICHIER_UTILISATEURS "../datas/users.dat"

user utilisateurConnecte;

int createUserAvecRole(const char *role) {
    pauseEtNettoie();
    puts("");
    color_header("********** CREATION D UN UTILISATEUR AVEC SON ROLE **********\n");
    puts("");
    user u;
    printf("Login (5 MAJ): ");
    scanf("%5s", u.login);
    if (!loginValide(u.login)) {
        color_error("Login invalide (5 majuscules requises).\n");
        return 0;
    }
    puts("Mot de passe (par defaut): admin123");
    strcpy(u.password, "admin123");
    printf("Prenom : ");
    scanf("%29s", u.prenom);
    printf("Nom : ");
    scanf("%29s", u.nom);
    printf("Telephone : ");
    scanf("%24s", u.telephone);
    strcpy(u.role, role);
    u.actif = 1;
    u.firstlogin = 1;
    FILE *f = fopen(FICHIER_UTILISATEURS, "ab");
    if (!f) {
        color_error("Erreur ouverture fichier.\n");
        return 0;
    }
    fwrite(&u, sizeof(user), 1, f);
    fclose(f);
    return 1;
}

int adminExiste() {
    FILE* f = fopen(FICHIER_UTILISATEURS, "rb");
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
    pauseEtNettoie();
    puts("");
    color_header("********** CREATION DE COMPTE **********\n");
    puts("");
    user u;
    printf("Login (5 MAJ MAX): ");
    scanf("%5s", u.login);
    if (!loginValide(u.login)) {
        color_error("Login invalide");
        return 0;
    }
    puts("Mot de passe (default): admin123");
    strcpy(u.password, "admin123");
    printf("Prenom : ");
    scanf("%29s", u.prenom);
    printf("Nom : ");
    scanf("%29s", u.nom);
    printf("Telephone : ");
    scanf("%24s", u.telephone);
    printf("Role (admin/pharmacien) : ");
    scanf("%14s", u.role);
    u.actif = 1;
    u.firstlogin = 1;
    FILE *f = fopen(FICHIER_UTILISATEURS, "ab");
    if (!f) {
        color_error("Erreur d'ouverture fichier\n");
        return 0;
    }
    size_t ecrits = fwrite(&u, sizeof(user), 1, f);
    fclose(f);
    if (ecrits != 1) {
        color_error("Erreur lors de l’écriture dans le fichier\n");
        return 0;
    }
    color_success("Utilisateur admin créé avec succès\n");
    return 1;
}

int connexion(user *u) {
    pauseEtNettoie();
    puts("");
    color_title("********** CONNEXION **********\n");
    puts("");
    char login[6];
    char pass[65];
    printf("Login (5 MAJ MAX): ");
    scanf("%5s", login);
    printf("Mot de passe : ");
    scanf("%64s", pass);
    FILE *f = fopen(FICHIER_UTILISATEURS, "rb+");
    if (!f) return 0;
    user tmp;
    long pos;
    while (fread(&tmp, sizeof(user), 1, f)) {
        if (strcmp(tmp.login, login) == 0 && strcmp(tmp.password, pass) == 0 && tmp.actif == 1) {
            *u = tmp;
            pos = ftell(f) - sizeof(user);
            if (tmp.firstlogin == 1) {
                color_success("Premiere connexion detectee, vous devez changer votre mot de passe.\n");
                char nouveau[65];
                do {
                    printf("\nNouveau mot de passe (6 caracteres minimum): \n");
                    scanf("%64s", nouveau);
                } while (strlen(nouveau) < 6);
                strcpy(tmp.password, nouveau);
                tmp.firstlogin = 0;
                fseek(f, pos, SEEK_SET);
                fwrite(&tmp, sizeof(user), 1, f);
                fflush(f);
                *u = tmp;
                color_success("Mot de passe modifie avec succes.\n");
            }
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void updatePassword(user* u) {
    pauseEtNettoie();
    puts("");
    color_header("********** CHANGEMENT DE MOT DE PASSE **********\n");
    puts("");
    char nouveau[65];
    do {
        printf("Nouveau mot de passe (6 caracteres minimum): ");
        scanf("%64s", nouveau);
    } while (strlen(nouveau) < 6);
    strcpy(u->password, nouveau);
    u->firstlogin = 0;
    FILE* f = fopen(FICHIER_UTILISATEURS, "rb+");
    if (!f) {
        color_error("Erreur d ouverture fichier pour mise a jour\n");
        return;
    }
    user tmp;
    while (fread(&tmp, sizeof(user), 1, f)) {
        if (strcmp(tmp.login, u->login) == 0) {
            fseek(f, -sizeof(user), SEEK_CUR);
            fwrite(u, sizeof(user), 1, f);
            fflush(f);
            break;
        }
    }
    fclose(f);
    color_success("Mot de passe mis a jour\n");
}

void afficherUtilisateurs() {
    pauseEtNettoie();
    puts("");
    color_header("********** AFFICHAGE DE TOUS LES UTILISATEURS **********\n");
    puts("");
    FILE* f = fopen(FICHIER_UTILISATEURS, "rb");
    if (!f) return;
    user u;
    while (fread(&u, sizeof(user), 1, f)) {
        printf("%s %s (%s) - %s [%s]\n", u.prenom, u.nom, u.login, u.role, u.actif ? "Actif" : "Bloque");
    }
    fclose(f);
}

int verifierLoginExiste(char login[]) {
    FILE* f = fopen(FICHIER_UTILISATEURS, "rb");
    if (!f) return 0;
    user u;
    while (fread(&u, sizeof(user), 1, f)) {
        if (strcmp(u.login, login) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int loginValide(char login[]) {
    return strlen(login) == 5;
}

int motDePasseValide(char password[]) {
    return strlen(password) >= 6;
}

int roleValide(char role[]) {
    return strcmp(role, "admin") == 0 || strcmp(role, "pharmacien") == 0;
}

void bloquerUtilisateur(char login[]) {
    pauseEtNettoie();
    puts("");
    color_header("********** BOQUAGE UN UTILISATEUR **********\n");
    puts("");
    FILE* f = fopen(FICHIER_UTILISATEURS, "rb+");
    if (!f) return;
    user u;
    while (fread(&u, sizeof(user), 1, f)) {
        if (strcmp(u.login, login) == 0) {
            u.actif = 0;
            fseek(f, -sizeof(user), SEEK_CUR);
            fwrite(&u, sizeof(user), 1, f);
            fflush(f);
            break;
        }
    }
    fclose(f);
}

void debloquerUtilisateur(char login[]) {
    pauseEtNettoie();
    puts("");
    color_header("********** DEBLOQUAGE UN UTILISATEUR **********\n");
    puts("");
    FILE* f = fopen(FICHIER_UTILISATEURS, "rb+");
    if (!f) return;
    user u;
    while (fread(&u, sizeof(user), 1, f)) {
        if (strcmp(u.login, login) == 0) {
            u.actif = 1;
            fseek(f, -sizeof(user), SEEK_CUR);
            fwrite(&u, sizeof(user), 1, f);
            fflush(f);
            break;
        }
    }
    fclose(f);
}

void supprimerUtilisateur(char login[]) {
    pauseEtNettoie();
    puts("");
    color_header("********** SUPPRESSION D UN UTILISATEUR **********\n");
    puts("");
    FILE* f = fopen(FICHIER_UTILISATEURS, "rb");
    FILE* temp = fopen("temp.dat", "wb");
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
    rename("temp.dat", FICHIER_UTILISATEURS);
}

void modifierUtilisateur(char login[]) {
    pauseEtNettoie();
    puts("");
    color_header("********** MODIFICATION D UN UTILISATEUR **********\n");
    puts("");
    FILE* f = fopen(FICHIER_UTILISATEURS, "rb+");
    if (!f) return;
    user u;
    while (fread(&u, sizeof(user), 1, f)) {
        if (strcmp(u.login, login) == 0) {
            printf("Prenom : ");
            lireChaine(u.prenom, 30);
            printf("Nom : ");
            lireChaine(u.nom, 30);
            printf("Telephone : ");
            lireChaine(u.telephone, 25);
            fseek(f, -sizeof(user), SEEK_CUR);
            fwrite(&u, sizeof(user), 1, f);
            fflush(f);
            break;
        }
    }
    fclose(f);
}

void menuGestionUtilisateurs() {
    pauseEtNettoie();
    int choix;
    char login[6];
    while (1) {
        puts("");
        color_header("MENU GESTION UTILISATEURS\n");
        puts("");
        printf("1. Ajouter un utilisateur\n");
        printf("2. Afficher tous les utilisateurs\n");
        printf("3. Modifier un utilisateur\n");
        printf("4. Bloquer un utilisateur\n");
        printf("5. Debloquer un utilisateur\n");
        printf("6. Supprimer un utilisateur\n");
        printf("7. Retour\n");
        choix = saisirEntierAvecMessage("Votre choix : ", 1, 7);
        switch (choix) {
            case 1: createUser(); break;
            case 2: afficherUtilisateurs(); break;
            case 3:
                printf("Login a modifier : ");
                scanf("%5s", login);
                viderBuffer();
                modifierUtilisateur(login);
                break;
            case 4:
                printf("Login a bloquer : ");
                scanf("%5s", login);
                viderBuffer();
                bloquerUtilisateur(login);
                break;
            case 5:
                printf("Login a debloquer : ");
                scanf("%5s", login);
                viderBuffer();
                debloquerUtilisateur(login);
                break;
            case 6:
                printf("Login a supprimer : ");
                scanf("%5s", login);
                viderBuffer();
                supprimerUtilisateur(login);
                break;
            case 7: return;
        }
        pauseEtNettoie();
    }
}
