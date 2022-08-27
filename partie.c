#include "header.h"

void creerJeu(jeu_t *jeu, char **pseudos, int nb)
{
    ///INITIALISATION DE LA PARTIE
    creerPioche(jeu);
    creerDefausse(jeu);
    creerJoueurs(pseudos, nb, jeu);
    creerBITMAP(jeu);
    creerPlateau(jeu);
    jeu->nbTour=0;
    jeu->selectionner=NULL;
    jeu->orMap=0;
    jeu->defausseNb=0;
}

void saveScores(jeu_t *jeu)
{
    for(int i=0; i<jeu->nbJoueurs; i++)
    {
        printf("%s %d\n", jeu->joueurs[i].pseudo, jeu->joueurs[i].score);
        saveScore(jeu->joueurs[i].pseudo, jeu->joueurs[i].score);
    }
}
void winSaboteur(BITMAP *buffer, jeu_t *jeu)//ss pgm victoire saboteur
{
    for(int i=0; i<jeu->nbJoueurs; i++)
    {
        if(jeu->joueurs[i].role==saboteur)
        {
            jeu->joueurs[i].score+=6;
        }
    }
    saveScores(jeu);
    ecranWinSaboteurs(buffer);
}
void winChercheur(BITMAP *buffer, jeu_t *jeu, int pepite)//ss pgm victoire chercheurs
{
    for(int i=0; i<jeu->nbJoueurs; i++)
    {
        printf("%s %d\n", jeu->joueurs[i].pseudo, jeu->joueurs[i].score);
        //saveScore(jeu->joueurs[i].pseudo, jeu->joueurs[i].score);
    }
    jeu->or1R=1;
    jeu->or2R=1;
    jeu->or3R=1;
    (void)afficherPlateau(buffer, jeu, NULL);
    typeCarte pepites[3];
    pepites[0]=jeu->or1;
    pepites[1]=jeu->or2;
    pepites[2]=jeu->or3;
    if(jeu->joueurs[jeu->tourJoueur].role==saboteur)
    {
        jeu->tourJoueur++;
        if(jeu->tourJoueur==jeu->nbJoueurs)
        {
            jeu->tourJoueur=0;
        }
    }
    jeu->joueurs[jeu->tourJoueur].score+=pepites[pepite]-2;
    for(int i=0; i<3; i++)
    {
        if(i==pepite)
        {
            continue;
        }
        jeu->tourJoueur++;
        if(jeu->tourJoueur==jeu->nbJoueurs)
        {
            jeu->tourJoueur=0;
        }
        if(jeu->joueurs[jeu->tourJoueur].role==saboteur)
        {
            i--;
            continue;
        }
        jeu->joueurs[jeu->tourJoueur].score+=pepites[i]-2;
    }
    saveScores(jeu);
    ecranWinChercheurs(buffer);
}
void debutPartie(char **pseudos, int nb, BITMAP *souris)//ss pgm partie
{
    BITMAP *buffer=create_bitmap(WIDTH, HEIGHT);
    jeu_t jeu;
    creerJeu(&jeu, pseudos, nb);
    typeCarte pioche=piocher(&jeu);
    jeu.NewCard=&(pioche);
    while(1)
    {
        int r=afficherPlateau(buffer, &jeu, souris);
        if(r==PIOCHER)
        {
            pioche=piocher(&jeu);
            if(pioche==vide)
            {
                winSaboteur(buffer, &jeu);
                return;
            }
            jeu.NewCard=&(pioche);
        }
        if(r==FINTOUR)
        {
            afficherPlateau(buffer, &jeu, souris);
            Sleep(500);
            jeu.nbTour++;
            jeu.tourJoueur++;
            jeu.defausseNb=0;
            pioche=piocher(&jeu);
            if(pioche==vide)
            {
                winSaboteur(buffer, &jeu);
                return;
            }
            jeu.NewCard=&(pioche);
            if(jeu.tourJoueur==jeu.nbJoueurs)
            {
                jeu.tourJoueur=0;
            }
            clear_bitmap(buffer);
            textprintf_centre(buffer, font, WIDTH/2, HEIGHT/2, makecol(255,255,255), "joueur %s: appuyez sur espace pour commencer votre tour!", jeu.joueurs[jeu.tourJoueur].pseudo);
            blit(buffer, screen, 0,0,0,0, WIDTH, HEIGHT);
            while(!key[KEY_SPACE])
            {

            }
            clear_bitmap(buffer);
        }
        if(r==SABOTEUR_WIN)
        {
            winSaboteur(buffer, &jeu);
            return;
        }
        if(r==CHERCHEUR_WIN1 || r==CHERCHEUR_WIN2 || r==CHERCHEUR_WIN3)
        {
            winChercheur(buffer, &jeu, r-3);
            return;
        }
    }
}
