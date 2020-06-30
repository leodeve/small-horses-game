// console output, very usefull for debugging with printf (*)
#include <stdio.h>
// string handling functions, also usefull
//#include <string.h>
// std c lib. a must for a good program (*)
#include <stdlib.h>
// if you do 3d you will mostly use sin/cos, here it goes (*)
#include <math.h>
// to use the time (useful to initialize a random (*))
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#elif linux
//Xlib (the lowest most possibility):
#include <X11/Xlib.h>

// for opengl this goes here (glu optional)
//#include <GL/glx.h>
//#include <GL/gl.h>
//#include <GL/glu.h> 
#else
#error Platform not supported
#endif



static int emplacement_canassons[2][2] = {{0, 0}, {0, 0}};
static int caseDuJeu[34];
static int num_joueur = 0;

//Prototypes des fonctions
void tourDeJeu(int);
void choixCouleur(int);
int choixAction(void);
void realiseAction(int, int, int);
void detecteVictoire(int);
void changeJoueur(int);
void affichageFinDeTour(void);
void color(int couleurDuTexte);

int main()
{
    
    color(6);
    printf("*****BIENVENUE DANS LE JEU DES PETITS CANASSONS*****\n");
    printf("* Jeu de 34 cases + 2 enclos + une zone arrivee\n");
    printf("* Les enclos correspondent a la case 0\n");
    printf("* La zone arrivee correspond a la case 100\n");
    printf("* Il faut faire 6 pour sortir un canasson de son enclos\n");
    printf("* Les canassons verts sortent case 1\n");
    printf("* Les canassons jaunes sortent case 18\n");
    printf("* Les canassons doivent revenir a leur case de depart\n");
    printf("\n");
    srand(time(NULL)); // Pour l'initialisation du random
    for (int i = 0; i < 34; i++)
    { // Initialisation des cases du jeu (toutes inoccupées)
        caseDuJeu[i] = 0;
    }
    while (1)
    {
        //color(7, 0); //gris clair
        color(6);
        printf("EMPLACEMENT DES CANASSONS :\n");
        printf("Joueur 1 - canasson 1 : case %d\n", emplacement_canassons[0][0]);
        printf("Joueur 1 - canasson 2 : case %d\n", emplacement_canassons[0][1]);
        printf("Joueur 2 - canasson 1 : case %d\n", emplacement_canassons[1][0]);
        printf("Joueur 2 - canasson 2 : case %d\n", emplacement_canassons[1][1]);
        printf("\n");
        tourDeJeu(num_joueur);
    }
    return 0;
}

void tourDeJeu(int num_joueur)
{
    choixCouleur(num_joueur);
    printf("JOUEUR %d\n", num_joueur + 1);
    int lancer = rand() % 6 + 1; // Nombre random modulo 6 + 1 pour éviter le 0
    printf("Le lancer est : %d\n", lancer);
    printf("\n");
    int num_action = choixAction(); // On récupère l'action choisie par l'utilisateur
    realiseAction(num_joueur, lancer, num_action);
    detecteVictoire(num_joueur);
    changeJoueur(num_joueur);
}

void choixCouleur(int num_joueur)
{
    if (num_joueur == 0)
    {
        color(2); //vert
    }
    else
    {
        color(4); //jaune
    }
}

int choixAction(void)
{
    int a = 0;
    while (a < 1 || a > 4)
    {
        printf("Choisir une action :\n");
        printf("1 : Sortir un canasson de son enclos (possible en cas de 6)\n");
        printf("2 : Avancer canasson 1\n");
        printf("3 : Avancer canasson 2\n");
        printf("4 : Ne rien faire\n");
        scanf("%d", &a); //l'utilisateur peut ici choisir son action
        if (a < 1 || a > 4)
        {
            printf("MAUVAISE SAISIE !\n");
        }
        printf("\n");
    }
    return a;
}

void realiseAction(int num_joueur, int lancer, int num_action)
{
    int caseDepart = 1 + 17 * num_joueur;
    switch (num_action)
    {
    case 1:
        if (lancer != 6)
        {
            printf("Action impossible, vous n'avez pas fait 6\n");
        }
        else if (caseDuJeu[caseDepart - 1] == 1)
        {
            printf("Action impossible, la case est occupee\n");
        }
        else if (emplacement_canassons[num_joueur][0] == 0)
        {
            printf("Le canasson 1 sort de l'enclos\n");
            emplacement_canassons[num_joueur][0] = caseDepart;
            caseDuJeu[caseDepart - 1] = 1;
        }
        else if (emplacement_canassons[num_joueur][1] == 0)
        {
            printf("Le canasson 2 sort de l'enlos\n");
            int caseDepart = 1 + 17 * num_joueur;
            emplacement_canassons[num_joueur][1] = caseDepart;
            caseDuJeu[caseDepart - 1] = 1;
        }
        else
        {
            printf("Tous les canassons sont deja sortis\n");
        }
        affichageFinDeTour();
        break;

    case 2:
        if (emplacement_canassons[num_joueur][0] == 0) //si le canasson est encore dans l'enclos
        {
            printf("Action impossible, le canasson est encore dans l'enclos\n");
            affichageFinDeTour();
            break;
        }
        int ancienne_case = emplacement_canassons[num_joueur][0];
        int nouvelle_case = (emplacement_canassons[num_joueur][0] + lancer) % 35 + floor((emplacement_canassons[num_joueur][0] + lancer) / 35); //case où on pourrait éventuellement aller
        if (ancienne_case == 100)                                                                                                               //si le canasson est deja dans la zone arrivee
        {
            printf("Action impossible, le canasson est deja dans la zone arrivee\n");
            affichageFinDeTour();
            break;
        }
        else if (caseDuJeu[nouvelle_case - 1] == 0) //si la nouvelle case est libre
        {
            emplacement_canassons[num_joueur][0] = nouvelle_case % 35 + floor(nouvelle_case / 35); //on positionne le canasson sur la nouvelle case
            if (emplacement_canassons[num_joueur][0] == 1 + 17 * num_joueur)                       //si le canasson revient à la case départ
            {
                emplacement_canassons[num_joueur][0] = 100; //on place le canasson dans la zone arrivee
                caseDuJeu[ancienne_case - 1] = 0;           //l'ancienne case devient libre
            }
            else
            {
                caseDuJeu[ancienne_case - 1] = 0; //l'ancienne case devient libre
                caseDuJeu[nouvelle_case - 1] = 1; //la nouvelle case devient occupee
            }
        }
        else
        {
            printf("Action impossible, la case visee est occupee\n");
        }
        affichageFinDeTour();
        break;

    case 3:
        if (emplacement_canassons[num_joueur][1] == 0) //si le canasson est encore dans l'enclos
        {
            printf("Action impossible, le canasson est encore dans l'enclos\n");
            affichageFinDeTour();
            break;
        }
        int ancienne_case2 = emplacement_canassons[num_joueur][1];
        int nouvelle_case2 = (emplacement_canassons[num_joueur][1] + lancer) % 35 + floor((emplacement_canassons[num_joueur][1] + lancer) / 35); //case où on pourrait éventuellement aller
        if (ancienne_case2 == 100)                                                                                                               //si le canasson est deja dans la zone arrivee
        {
            printf("Action impossible, le canasson est deja dans la zone arrivee\n");
            affichageFinDeTour();
            break;
        }
        else if (caseDuJeu[nouvelle_case2 - 1] == 0) //si la nouvelle case est libre
        {
            emplacement_canassons[num_joueur][1] = nouvelle_case2 % 35 + floor(nouvelle_case2 / 35); //on positionne le canasson sur la nouvelle case
            if (emplacement_canassons[num_joueur][1] == 1 + 17 * num_joueur)                         //si le canasson revient à la case départ
            {
                emplacement_canassons[num_joueur][1] = 100; //on place le canasson dans la zone arrivee
                caseDuJeu[ancienne_case2 - 1] = 0;          //l'ancienne case devient libre
            }
            else
            {
                caseDuJeu[ancienne_case2 - 1] = 0; //l'ancienne case devient libre
                caseDuJeu[nouvelle_case2 - 1] = 1; //la nouvelle case devient occupee
            }
        }
        else
        {
            printf("Action impossible, la case visee est occupee\n");
        }
        affichageFinDeTour();
        break;

    case 4:
        affichageFinDeTour();
        break;
    }
}

void detecteVictoire(int num_joueur)
{
    if (emplacement_canassons[num_joueur][0] == 100 && emplacement_canassons[num_joueur][1] == 100)
    {
        printf("------->>> VICTOIRE JOUEUR %d", num_joueur + 1);
        printf("\n");
        exit(0);
    }
}

void changeJoueur(int n)
{
    if (n == 0)
    {
        num_joueur = 1;
    }
    else
    {
        num_joueur = 0;
    }
}

void affichageFinDeTour(void)
{
    printf("FIN DU TOUR\n");
    printf("--------------------------------------------------------------\n");
    printf("\n");
}

/* void color(int couleurDuTexte, int couleurDuFond)
{
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDuFond * 16 + couleurDuTexte);
} */

void color(int couleurDuTexte){
    switch(couleurDuTexte){
        case 0: 
            printf("\033[0;31m"); //Red
            break;
        case 1:
            printf("\033[1;31m"); //Bold Red
            break;
        case 2: 
            printf("\033[0;32m"); //Green
            break;
        case 3:
            printf("\033[1;32m"); //Bold Green
            break;
        case 4: 
            printf("\033[0;33m"); //Yellow
            break;
        case 5:
            printf("\033[1;33m"); //Bold Yellow
            break;
        case 6: 
            printf("\033[0;34m"); //Blue
            break;
        case 7:
            printf("\033[1;34m"); //Bold Blue
            break;
        case 8: 
            printf("\033[0;35m"); //Magenta
            break;
        case 9:
            printf("\033[1;35m"); //Bold Magenta
            break;
        case 10: 
            printf("\033[0;36m"); //Cyan
            break;
        case 11:
            printf("\033[1;36m"); //Bold Cyan
            break;
        case 12:
            printf("\033[0m"); //Reset
            break;
    }

}
