#ifndef USERS_H
#define USERS_H

#define MAX_LOGIN 6
#define MAX_ROLE 15
#define MAX_PASS 65
#define MAX_NOM 30
#define MAX_TEL 25

typedef struct {
    char login[MAX_LOGIN];
    char password[MAX_PASS];
    char prenom[MAX_NOM];
    char nom[MAX_NOM];
    char telephone[MAX_TEL];
    char role[MAX_ROLE];
    int actif;
    int firstlogin;
} user;

int loginValide(char login[]);
int adminExiste();
int createUser();
int connexion(user *u);
void afficherResumeUser(user u);
void updatePassword(user *u);
void bloquerUtilisateur(const char *login);
void debloquerUtilisateur(const char *login);
void supprimerUtilisateur(const char *login);
void afficherTousUtilisateurs();
void menuGestionUtilisateurs();

void menuPharmacien(user u);
void menuAdmin(user u);

#endif
