#ifndef USERS_H
#define USERS_H

#define FICHIER_UTILISATEURS "datas/users.dat"

typedef struct {
    int id;
    char login[6];
    char password[65];
    char prenom[30];
    char nom[30];
    char telephone[25];
    char role[15];
    int actif;
    int firstlogin;
} user;

int createUserAvecRole(const char *role);
int connexion(user* u);
int adminExiste();
int createUser();
void updatePassword(user* u);
void afficherUtilisateurs();
void supprimerUtilisateur(char login[]);
void bloquerUtilisateur(char login[]);
void debloquerUtilisateur(char login[]);
void modifierUtilisateur(char login[]);
void menuGestionUtilisateurs();
int loginValide(char login[]);
int motDePasseValide(char password[]);
int verifierLoginExiste(char login[]);
int roleValide(char role[]);
extern user utilisateurConnecte;


#endif
