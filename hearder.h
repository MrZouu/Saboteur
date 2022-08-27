#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#include <allegro.h>
#include <winalleg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <dirent.h>
#define HEIGHT 800
#define WIDTH 800
#define NBIM 26
#define SABOTEUR_WIN -1
#define FINTOUR 1
#define PIOCHER 2
#define CHERCHEUR_WIN1 3
#define CHERCHEUR_WIN2 4
#define CHERCHEUR_WIN3 5

///STRUCTURES
typedef enum
{
    vide=0,
    chercheur=1,
    saboteur=2,
    pepiteOr1=3,
    pepiteOr2=4,
    pepiteOr3=5,
    entreeMine=6,
    galerieT1=7,
    galerieT2=8,
    galerieImpasse=9,
    galerieCarr=10,
    galerieChemin=11,
    charioRep=12,
    charioCass=13,
    piocheRep=14,
    piocheCass=15,
    lanterneRep=16,
    lanterneCass=17,
    carteMap=18,
    outilsCass=19,
    lantPiochRep=20,
    lantCharRep=21,
    charPiochRep=22,
    eboulement=23,
    mur=24,
    orVerso=25,
    rien=26,

}typeCarte;

typedef struct paquetCarte
{
    typeCarte carte;
    int nb;
}paquetCarte_t;

typedef struct joueur
{
    char pseudo[20];
     typeCarte role;
     typeCarte carte[6];
     typeCarte action[3];
     int score;
}joueur_t;

typedef struct pile
{
    typeCarte carte;
    struct pile *suivant;
}pile_t;

typedef struct bitmapCarte
{
    typeCarte carte;
    BITMAP *image;
    char nomIm[255];
    BITMAP *MiniIm;
}bitmapCarte_t;

typedef struct jeu
{
    int nbJoueurs;
    joueur_t joueurs[4];
    struct pile *pioche;
    typeCarte defausse;
    int defausseNb;
    paquetCarte_t paquet[17];
    bitmapCarte_t images[NBIM];
    typeCarte plateau[5][7];
    typeCarte or1, or2, or3;
    int or1R, or2R, or3R;
    int orMap;
    int tourJoueur;
    int nbTour;
    typeCarte *selectionner;
    typeCarte *NewCard;
}jeu_t;

typedef struct chercheur{
    int posLig;
    int posCol;
    int depLig;
    int depCol;
    BITMAP *chercheur;
}t_chercheur;

///PROTOTYPES
int creerNbJoueurs(BITMAP *buffer, BITMAP *souris, BITMAP *image);
char *nomJoueurs(BITMAP *buffer, int nombreJoueurs, BITMAP *image);
void menuBvn(BITMAP *buffer, BITMAP *demon, FONT* police);
void creerPaquet(jeu_t *jeu);
BITMAP *bitmapPourCarte(jeu_t* jeu, typeCarte carte);
BITMAP *bitmapPourMiniCarte(jeu_t* jeu, typeCarte carte);
void creerBITMAP(jeu_t *jeu);
void creerDefausse(jeu_t *jeu);
void ajout(typeCarte carte, jeu_t *jeu);
void creerPioche(jeu_t *jeu);
typeCarte piocher(jeu_t *jeu);
int estChemin(typeCarte tc);
int estAction(typeCarte tc);
void creerJoueurs(char **pseudo, int nbJoueur, jeu_t *jeu);
void creerPlateau(jeu_t *jeu);
void afficherOr(jeu_t *jeu, BITMAP *buffer, int taille_x, int taille_y);
void afficherMine(jeu_t *jeu, BITMAP *buffer, int taille_x, int taille_y);
void selectionnerCarte(BITMAP *buffer, jeu_t *jeu, int taille_x, int taille_y);
int deposerCarteChem(BITMAP *buffer, jeu_t *jeu, int taille_x, int taille_y);
int deposerCarteAction(BITMAP *buffer, jeu_t *jeu, int taille_x, int taille_y);
int ajouterAction(typeCarte action, jeu_t *jeu, int joueur);
int deposerMap(jeu_t *jeu, int taille_x, int taille_y);
int poserCarte(jeu_t *jeu);
int dragAndDrop(BITMAP *buffer, jeu_t *jeu, BITMAP *souris, int taille_x, int taille_y);
void afficherPanneJoueurs(BITMAP *buffer, jeu_t *jeu);
int afficherPlateau(BITMAP *buffer, jeu_t *jeu, BITMAP *souris);
void creerJeu(jeu_t *jeu, char **pseudos, int nb);
void debutPartie(char **pseudos, int nb, BITMAP *souris);
void ecranWinChercheurs(BITMAP *buffer);
void ecranWinSaboteurs(BITMAP *buffer);
void winChercheur(BITMAP *buffer, jeu_t *jeu, int pepite);
void winSaboteur(BITMAP *buffer, jeu_t *jeu);
#endif // HEADER_H_INCLUDED
