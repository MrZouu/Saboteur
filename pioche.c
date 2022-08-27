#include "header.h"

void creerPaquet(jeu_t *jeu)//on attribue a chaque type de carte le nombre de fois qu'elle se trouve dans le paquet
{
    paquetCarte_t paquet[17]={
    {galerieT1, 9},{galerieT2, 9}, {galerieImpasse, 9},
    {galerieCarr, 9},{galerieChemin, 9}, {charioRep, 2},
    {charioCass, 2},{piocheRep, 2}, {piocheCass, 2},
    {lanterneRep, 2},{lanterneCass, 2}, {carteMap, 2},
    {outilsCass, 2},{lantPiochRep, 2}, {lantCharRep, 2},
    {charPiochRep, 2},{eboulement, 2},
    };
    for(int i=0; i<17; i++)
    {
        jeu->paquet[i]=paquet[i];
    }
}

BITMAP *bitmapPourMiniCarte(jeu_t* jeu, typeCarte carte) //recuperer l'image qui correspond a une carte passee en parametres
{
    for(int i=0; i<NBIM; i++)
    {
        if (jeu->images[i].carte==carte)
        {
            return jeu->images[i].MiniIm;
        }
    }
    return NULL;
}

BITMAP *bitmapPourCarte(jeu_t* jeu, typeCarte carte) //recuperer l'image qui correspond a une carte passee en parametres
{
    for(int i=0; i<NBIM; i++)
    {
        if (jeu->images[i].carte==carte)
        {
            return jeu->images[i].image;
        }
    }
    return NULL;
}
void creerBITMAP(jeu_t *jeu)//on assimile a chaque type de carte son image correspondante dans le dossier
{
    bitmapCarte_t image[NBIM]={
    {vide, NULL, "assets/galerie_verso.bmp"},
    {galerieT1, NULL, "assets/galerie_croisement_T_horizontal.bmp"},
    {galerieT2, NULL, "assets/galerie_croisement_T_vertical.bmp"},
    {galerieImpasse, NULL, "assets/galerie_impasse.bmp"},
    {galerieCarr, NULL, "assets/galerie_carrefour.bmp"},
    {galerieChemin, NULL, "assets/galerie_chemin.bmp"},
    {charioRep, NULL, "assets/action_chariot_reparer.bmp"},
    {charioCass, NULL, "assets/action_chariot_casser.bmp"},
    {piocheRep, NULL, "assets/action_pioche_reparer.bmp"},
    {piocheCass, NULL, "assets/action_pioche_casser.bmp"},
    {lanterneRep, NULL, "assets/action_lanterne_reparer.bmp"},
    {lanterneCass, NULL, "assets/action_lanterne_casser.bmp"},
    {carteMap, NULL, "assets/action map.bmp"},
    {outilsCass, NULL, "assets/action_tous_outils_casser.bmp"},
    {lantPiochRep, NULL, "assets/action_lanterne_pioche_reparer.bmp"},
    {lantCharRep, NULL, "assets/action_lanterne_chariot_reparer.bmp"},
    {charPiochRep, NULL, "assets/action_chariot_pioche_repares.bmp"},
    {eboulement, NULL, "assets/action_eboulement.bmp"},
    {chercheur, NULL, "assets/personnage_chercheur.bmp"},
    {saboteur, NULL, "assets/personnage_saboteur.bmp"},
    {pepiteOr1, NULL, "assets/pepite_1.bmp"},
    {pepiteOr2, NULL, "assets/pepite_2.bmp"},
    {pepiteOr3, NULL, "assets/pepite_3.bmp"},
    {entreeMine, NULL, "assets/entree_mine.bmp"},
    {orVerso, NULL, "assets/pepite_verso.bmp"},
    };
    for(int i=0; i<NBIM; i++)
    {
        jeu->images[i]=image[i];
        jeu->images[i].image=load_bitmap(image[i].nomIm,NULL);//on assimile a chaque type de carte son image
        char *miniName=malloc(sizeof(char)*(strlen(image[i].nomIm)+5));
        strcpy(miniName, image[i].nomIm);
        strcpy(&(miniName[strlen(miniName)-4]), "_mini.bmp");//meme chose avec les mini cartes pour les penalites
        jeu->images[i].MiniIm=load_bitmap(miniName, NULL);
    }
}

void creerDefausse(jeu_t *jeu)
{
    jeu->defausse=rien;//on initialise la defausse a rien
}

void ajout(typeCarte carte, jeu_t *jeu)//ajout d'un maillon dans une pile, inspiré du cours de Mme Rendler
{
    pile_t *tmp=jeu->pioche;
    pile_t *tmp2=malloc(sizeof(pile_t));
    if(tmp2==NULL)
    {
        exit(0);
    }
    tmp2->carte=carte;
    tmp2->suivant=NULL;
    if(tmp==NULL)
    {
        jeu->pioche=tmp2;
        return;
    }
    while(tmp->suivant!=NULL)
    {
        tmp=tmp->suivant;
    }
    tmp->suivant=tmp2;
}
void creerPioche(jeu_t *jeu)
{
    jeu->pioche=NULL;
    creerPaquet(jeu);
    srand(time(NULL));
    for(int i=0; i<69; i++)//on parcours toutes les cartes
    {
        int t=rand()%17;//on tire un nombre au hasard dans les 17 types de cartes differents
        while(jeu->paquet[t].nb==0)
        {
            t=rand()%17;//si il n'y a plus cette carte dans le paquet, on retire un nouveau nombre
        }
        ajout(jeu->paquet[t].carte, jeu);//on ajoute cette carte du paquet dans la pioche
        jeu->paquet[t].nb--;//la carte du type tiré se voit retirée du paquet
    }
}

 typeCarte piocher(jeu_t *jeu)//on pioche une nouvelle carte
{
    if(jeu->pioche==NULL)
    {
        return vide;//detection de la pioche vide
    }
    typeCarte tc=jeu->pioche->carte; //on pioche la carte au dessus de la pioche
    jeu->pioche=jeu->pioche->suivant;//on decale toutes les autres cartes de 1 cran
    return tc;//on renvoie la carte piochee
}

int estChemin(typeCarte tc)//on verifie si la carte est un chemin...
{
    if((tc>=galerieT1 && tc<=galerieChemin) || tc==eboulement)
    {
        return 1;
    }
    return 0;
}

int estAction(typeCarte tc)//...ou une carte action
{
    if(tc>=charioRep && tc<eboulement)
    {
        return 1;
    }
    return 0;
}
