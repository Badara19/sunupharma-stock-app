typedef struct {
    char login[6];
    char password[20];
    char prenom[30];
    char nom[30];
    char telephone[25];
    char role[15];
    int actif;
    int firstlogin;
}user;

int loginvalide(char login[]);
int createuser();
int connexion(user *p);
void updatepassword(user *p);
void bloqueuser(char login[6]);
void debloqueuser(char login[6]);
void afficheuser(user u);
void listeuser();
