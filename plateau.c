#include "header.h"

void creerPlateau(jeu_t *jeu)//creation du plateau et initialisation des pepites
{
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<7; j++)
        {
            jeu->plateau[i][j]=vide;//initialisation de la zone de jeu
        }
    }
    int r=rand()%3; //permet de mettre les pepites aleatoirement entre 0 et 2...
    jeu->or1=pepiteOr1+r;
    int r2=r;
    while(r2==r)//...pour tomber sur 3 chiffres differents de ceux qu'on possede deja
    {
        r2=rand()%3;
    }
    jeu->or2=pepiteOr1+r2;
    int r3=r;
    while(r3==r||r3==r2)
    {
        r3=rand()%3;
    }
    jeu->or3=pepiteOr1+r3;
    jeu->or1R=0;
    jeu->or2R=0;
    jeu->or3R=0;

}

void afficherOr(jeu_t *jeu, BITMAP *buffer, int taille_x, int taille_y)//affichage des pepites
{
    if(jeu->or1R==0 && jeu->orMap!=1)
    {
            draw_sprite(buffer, bitmapPourCarte(jeu, orVerso), WIDTH-taille_x, HEIGHT-(taille_y*5/1.25)-taille_y/2);//affichage dos de la carte or
    }
    else
    {
        draw_sprite(buffer, bitmapPourCarte(jeu, jeu->or1), WIDTH-taille_x, HEIGHT-(taille_y*5/1.25)-taille_y/2);//affichage carte or
    }
    if(jeu->or2R==0 && jeu->orMap!=2)
    {
            draw_sprite(buffer, bitmapPourCarte(jeu, orVerso), WIDTH-taille_x, HEIGHT-(taille_y*5/2)-taille_y/2);
    }
    else
    {
        draw_sprite(buffer, bitmapPourCarte(jeu, jeu->or2), WIDTH-taille_x, HEIGHT-(taille_y*5/2)-taille_y/2);
    }
    if(jeu->or3R==0 && jeu->orMap!=3)
    {
        draw_sprite(buffer, bitmapPourCarte(jeu, orVerso), WIDTH-taille_x, HEIGHT-(taille_y*5/4.75)-taille_y/2);
    }
    else
    {
        draw_sprite(buffer, bitmapPourCarte(jeu, jeu->or3), WIDTH-taille_x, HEIGHT-(taille_y*5/4.75)-taille_y/2);
    }
}

void afficherMine(jeu_t *jeu, BITMAP *buffer, int taille_x, int taille_y)//affichage de la mine
{
    int depart_x=WIDTH-8*taille_x, depart_y=HEIGHT-5*taille_y;
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<7; j++)
        {
            draw_sprite(buffer,bitmapPourCarte(jeu, jeu->plateau[i][j]), depart_x, depart_y);//on affiche le plateau avec des dos de cartes
            depart_x+=taille_x;//decalage d'une colonne a la fois pour remplir une ligne
        }
        depart_x-=taille_x*7;//on revient a la premiere colonne
        depart_y+=taille_y;//on passe a la ligne suivante
    }

}
void selectionnerCarte(BITMAP *buffer, jeu_t *jeu, int taille_x, int taille_y)
{
    int depart_x=0;
    int depart_y=15;
    int fin_x=taille_x*6;
    int fin_y=taille_y;
    if(jeu->selectionner==NULL && mouse_x>=depart_x && mouse_x<=fin_x &&mouse_y>=depart_y && mouse_y<=fin_y)//coordonnees des cartes du joueurs affichees
        {
            int sel=mouse_x/taille_x;
            jeu->selectionner=&(jeu->joueurs[jeu->tourJoueur].carte[sel]);//selectionne une des 6 cartes du joueur
        }
    if(jeu->selectionner==NULL && mouse_x>=WIDTH-taille_x*2 && mouse_x<=WIDTH-taille_x &&mouse_y>=0 && mouse_y<=taille_y)//coordonees de la pioche
    {
        jeu->selectionner=jeu->NewCard;//selectionne la carte affichee au niveau de la pioche
    }
}

int verifierTunnel(jeu_t *jeu)
{
    if(jeu->plateau[2][0]==vide)
    {
        return 0;
    }
    return verifierTunnel2(jeu, 0, 2, 0);
}

int verifierTunnel2(jeu_t *jeu, int x, int y, int depart)//verification de la possibilite des placements de cartes en fonction du type de carte
{
    typeCarte *carteGauche=NULL;
    typeCarte *carteDroite=NULL;
    typeCarte *carteHaut=NULL;
    typeCarte *carteBas=NULL;
    typeCarte carte=jeu->plateau[y][x];
    int res=-1;
    if(carte==galerieImpasse || carte==eboulement)
    {
        return res;
    }
    if(x>0)//on se place dans les coordonnes de plateau pour chaque carte
    {
        carteGauche=&(jeu->plateau[y][x-1]);
    }
    if(x<6)
    {
        carteDroite=&(jeu->plateau[y][x+1]);
    }
    if(y>0)
    {
        carteHaut=&(jeu->plateau[y-1][x]);
    }
    if(y<4)
    {
        carteBas=&(jeu->plateau[y+1][x]);
    }
    if(depart!=0 && (carte==galerieChemin || carte==galerieCarr || carte==galerieT2))
    {
        if(carteGauche!=NULL && *(carteGauche)==vide)
        {
            res=0;//chemin possible
        }
        else if(carteGauche!=NULL)
        {
            int res1=verifierTunnel2(jeu, x-1, y, 2);
            if(res1>=0)//trouver pepite
            {
                return res1;
            }
            else if(res1==0)//pas trouve pepite mais chemin ou peut construire
            {
                res=res1;
            }
        }

    }
    if(depart!=2 && (carte==galerieChemin || carte==galerieCarr || carte==galerieT1 || carte==galerieT2))
    {
        puts("check droite");
        printf("%d\n", y);
        if(carteDroite==NULL)
        {
            puts("arrivee fin");
            if(y<2)
            {
                puts("1");
                return CHERCHEUR_WIN1;
            }
            else if(y==2)
            {
                puts("2");
                return CHERCHEUR_WIN2;
            }
            else if(y>2)
            {
                puts("3");
                return CHERCHEUR_WIN3;
            }
        }
        if(carteDroite!=NULL && *(carteDroite)==vide)
        {
            res=0;//chemin possible
        }
        else if(carteDroite!=NULL)
        {
            int res1=verifierTunnel2(jeu, x+1, y, 0);
            if(res1>=0)//trouver pepite
            {
                return res1;
            }
            else if(res1==0)//pas trouve pepite mais chemin ou peut construire
            {
                res=res1;
            }
        }
    }
    if(depart!=1 && (carte==galerieCarr || carte==galerieT1 || carte==galerieT2))
    {
        if(carteHaut!=NULL && *(carteHaut)==vide)
        {
            res=0;//chemin possible
        }
        else if(carteHaut!=NULL)
        {
            int res1=verifierTunnel2(jeu, x, y-1, 3);
            if(res1>=0)//trouver pepite
            {
                return res1;
            }
            else if(res1==0)//pas trouve pepite mais chemin ou peut construire
            {
                res=res1;
            }
        }
    }
    if(depart!=3 && (carte==galerieCarr || carte==galerieT1))
    {
        if(carteBas!=NULL && *(carteBas)==vide)
        {
            res=0;//chemin possible
        }
        else if(carteBas!=NULL)
        {
            int res1=verifierTunnel2(jeu, x, y+1, 1);
            if(res1>=0)//trouver pepite
            {
                return res1;
            }
            else if(res1==0)//pas trouve pepite mais chemin ou peut construire
            {
                res=res1;
            }
        }
    }
    printf("%d\n%d\n", carte, res);
    return res;
}

int verifierChemin(jeu_t *jeu, int x, int y)//en fonction de la carte, on peut la placer ou nom a coter d'une autre
{
    typeCarte *carteGauche=NULL;
    typeCarte *carteDroite=NULL;
    typeCarte *carteHaut=NULL;
    typeCarte *carteBas=NULL;

    if(x>0)//on se place dans les coordonnes de plateau pour chaque carte
    {
        carteGauche=&(jeu->plateau[y][x-1]);
    }
    if(x<6)
    {
        carteDroite=&(jeu->plateau[y][x+1]);
    }
    if(y>0)
    {
        carteHaut=&(jeu->plateau[y-1][x]);
    }
    if(y<4)
    {
        carteBas=&(jeu->plateau[y+1][x]);
    }
    switch(*(jeu->selectionner))
    {
    case galerieImpasse://pour la carte impasse, seulement a gauche
        if(carteGauche==NULL || *(carteGauche)==vide || *(carteGauche)==eboulement)//on determine les conditions en fonction de la carte et de quelles cartes peuvent se trouver a coter
        {
            return 0;
        }
        else
        {
            return 1;
        }
    case galerieT2://pour la T2, a gauche a droite et en haut
        if((carteGauche!=NULL &&(*(carteGauche)==galerieCarr || *(carteGauche)==galerieChemin || *(carteGauche)==galerieT1 || *(carteGauche)==galerieT2)) ||
           (carteDroite!=NULL &&(*(carteDroite)==galerieCarr || *(carteDroite)==galerieChemin || *(carteDroite)==galerieT2)) ||
           (carteHaut!=NULL &&(*(carteHaut)==galerieCarr || *(carteHaut)==galerieT1)))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case galerieChemin://pour le chemin, a gauche et a droite
        if((carteGauche!=NULL &&(*(carteGauche)==galerieCarr || *(carteGauche)==galerieChemin || *(carteGauche)==galerieT1 || *(carteGauche)==galerieT2)) ||
           (carteDroite!=NULL &&(*(carteDroite)==galerieCarr || *(carteDroite)==galerieChemin || *(carteDroite)==galerieT2)))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case galerieT1://pour la galerie T1, en haut, en bas et a droite
        if((carteDroite!=NULL &&(*(carteDroite)==galerieCarr || *(carteDroite)==galerieChemin || *(carteDroite)==galerieT2))||
            (carteHaut!=NULL &&(*(carteHaut)==galerieCarr || *(carteHaut)==galerieT1)) ||
           (carteBas!=NULL &&(*(carteBas)==galerieCarr || *(carteBas)==galerieT1 || *(carteBas)==galerieT2)))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case galerieCarr://pour lecarrefour, en haut, en bas, a gauche et a droite
        if((carteGauche!=NULL &&(*(carteGauche)==galerieCarr || *(carteGauche)==galerieChemin || *(carteGauche)==galerieT1 || *(carteGauche)==galerieT2))||
           (carteDroite!=NULL &&(*(carteDroite)==galerieCarr || *(carteDroite)==galerieChemin || *(carteDroite)==galerieT2))||
            (carteHaut!=NULL &&(*(carteHaut)==galerieCarr || *(carteHaut)==galerieT1)) ||
           (carteBas!=NULL &&(*(carteBas)==galerieCarr || *(carteBas)==galerieT1 || *(carteBas)==galerieT2)))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case eboulement:
        if((carteGauche!=NULL &&(*(carteGauche)==galerieCarr || *(carteGauche)==galerieChemin || *(carteGauche)==galerieT1 || *(carteGauche)==galerieT2))||
           (carteDroite!=NULL &&(*(carteDroite)==galerieCarr || *(carteDroite)==galerieChemin || *(carteDroite)==galerieT2))||
            (carteHaut!=NULL &&(*(carteHaut)==galerieCarr || *(carteHaut)==galerieT1)) ||
           (carteBas!=NULL &&(*(carteBas)==galerieCarr || *(carteBas)==galerieT1 || *(carteBas)==galerieT2)))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    default:
        return 0;
    }
}

int deposerCarteChem(BITMAP *buffer, jeu_t *jeu, int taille_x, int taille_y)//ss pgm pour poser une carte chemin
{
    int depart_x=WIDTH-8*taille_x, depart_y=HEIGHT-5*taille_y;
    int fin_x=WIDTH-taille_x;
    int fin_y=HEIGHT;
    int x=-1, y=-1;
    if(jeu->defausseNb!=0)//si on a commencé a defausser, on ne peut pas poser de carte chemin
    {
        return 0;
    }
    if(jeu->selectionner!=NULL)
        {
            for(int i=0; i<3; i++)
            {
                typeCarte carte=jeu->joueurs[jeu->tourJoueur].action[i];//si le joueur possede une penalité, il ne peut pas poser de carte chemin
                if(carte==charioCass)
                {
                    return 0;
                }
                if(carte==lanterneCass)
                {
                    return 0;
                }
                if(carte==piocheCass)
                {
                    return 0;
                }
                if(carte==outilsCass)
                {
                    return 0;
                }
            }
            if(estChemin(*(jeu->selectionner))==1)//si le joueur selectionne une carte chemin
            {
                if(mouse_x>fin_x || mouse_x<depart_x || mouse_y>fin_y || mouse_y<depart_y)//mais qu'il ne pose pas dans le plateau
                {
                    return 0;//on renvoie 0
                }
                x=(mouse_x-depart_x)/(taille_x);
                y=(mouse_y-depart_y)/taille_y;
                if(jeu->plateau[y][x]!=vide)//si une carte est deja posee, on ne peut pas poser par dessus
                {
                    return 0;
                }
                if(jeu->plateau[2][0]==vide && y==2 && x==0)//si la position a droite de l'entre de la mine est vide
                {
                    if(*(jeu->selectionner)==galerieImpasse || *(jeu->selectionner)==galerieT1)//mais qu'on selection une carte impasse
                    {
                        return 0;//renvoi 0
                    }
                    jeu->plateau[y][x]=*(jeu->selectionner);//ou si on selectionne une autre carte chemin
                    return 1;//renvoi 1
                }
                else if(jeu->plateau[2][0]!=vide)//si une carte est deja placee a l'entree
                {
                    if(verifierChemin(jeu, x, y)==1)//et que on verifie que la carte selectionner peut e poser a coter de celle deja posée
                    {
                        jeu->plateau[y][x]=*(jeu->selectionner);
                        return 1;//renvoi 1
                    }
                    return 0;
                }
            }
        }
        return 0;
}

int deposerCarteAction(BITMAP *buffer, jeu_t *jeu, int taille_x, int taille_y)//ss pgm pour poser une carte action
{
    int size=100, space=20;
    int depart_y=HEIGHT-5*taille_y;
    int depart_x=20;
    int fin_x=120;
    int fin_y=depart_y+jeu->nbJoueurs*(size+space);
    int x=-1;
    if(jeu->defausseNb!=0)//si on commence a defausser on ne peut pas poser une carte action
    {
        return 0;
    }
    if(jeu->selectionner!=NULL)
        {
            if(estAction(*(jeu->selectionner))==1)//si on sekectionne une carte action
            {
                if(mouse_x>fin_x || mouse_x<depart_x || mouse_y>fin_y || mouse_y<depart_y)//mais qu'on est pas dans les coordonnees adequates
                {
                    return 0;//renvoi 0
                }
                x=(mouse_y-depart_y)/(size+space);
                return ajouterAction(*(jeu->selectionner), jeu, x);;
            }
        }
        return 0;
}

int ajouterAction(typeCarte action, jeu_t *jeu, int joueur)
{
    for(int i=0; i<3; i++)
    {
        if(jeu->joueurs[joueur].action[i]==action)//on ne peut pas poser 2 cartes actions identiques au meme endroit
        {
            return 0;
        }
    }
    if(action==charioRep)//dans ces lignes de code, on etudie chaque cas possible pour les actions, si il a telle carte cassee et qu'il pose la carte correspondante pour reparer,...
    {
        for(int i=0; i<3; i++)//...alors on repare etc etc
        {
            if(jeu->joueurs[joueur].action[i]==charioCass)
            {
                jeu->joueurs[joueur].action[i]=rien;
                return 1;
            }
            if(jeu->joueurs[joueur].action[i]==outilsCass)
            {
                jeu->joueurs[joueur].action[i]=lanterneCass;
                for(int j=0; j<3;j++)
                {
                    if(jeu->joueurs[joueur].action[j]==rien)
                    {
                        jeu->joueurs[joueur].action[j]=piocheCass;
                        return 1;
                    }
                }
                return 1;
            }
        }
        return 0;
    }
    if(action==lanterneRep)
    {
        for(int i=0; i<3; i++)
        {
            if(jeu->joueurs[joueur].action[i]==lanterneCass)
            {
                jeu->joueurs[joueur].action[i]=rien;
                return 1;
            }
            if(jeu->joueurs[joueur].action[i]==outilsCass)
            {
                jeu->joueurs[joueur].action[i]=charioCass;
                for(int j=0; j<3;j++)
                {
                    if(jeu->joueurs[joueur].action[j]==rien)
                    {
                        jeu->joueurs[joueur].action[j]=piocheCass;
                        return 1;
                    }
                }
                return 1;
            }
        }
        return 0;
    }
    if(action==piocheRep)
    {
        for(int i=0; i<3; i++)
        {
            if(jeu->joueurs[joueur].action[i]==piocheCass)
            {
                jeu->joueurs[joueur].action[i]=rien;
                return 1;
            }
            if(jeu->joueurs[joueur].action[i]==outilsCass)
            {
                jeu->joueurs[joueur].action[i]=lanterneCass;
                for(int j=0; j<3;j++)
                {
                    if(jeu->joueurs[joueur].action[j]==rien)
                    {
                        jeu->joueurs[joueur].action[j]=charioCass;
                        return 1;
                    }
                }
                return 1;
            }
        }
        return 0;
    }
    int ret=0;
    if(action==lantCharRep)
    {
        for(int i=0; i<3; i++)
        {
            if(jeu->joueurs[joueur].action[i]==outilsCass)
            {
                jeu->joueurs[joueur].action[i]=piocheCass;
                return 1;
            }
            if(jeu->joueurs[joueur].action[i]==charioCass)
            {
                ret=1;
                jeu->joueurs[joueur].action[i]=rien;
            }
            if(jeu->joueurs[joueur].action[i]==lanterneCass)
            {
                ret=1;
                jeu->joueurs[joueur].action[i]=rien;
            }
        }
        return ret;
    }

    if(action==lantPiochRep)
    {
        for(int i=0; i<3; i++)
        {
            if(jeu->joueurs[joueur].action[i]==outilsCass)
            {
                jeu->joueurs[joueur].action[i]=charioCass;
                return 1;
            }
            if(jeu->joueurs[joueur].action[i]==piocheCass)
            {
                ret=1;
                jeu->joueurs[joueur].action[i]=rien;
            }
            if(jeu->joueurs[joueur].action[i]==lanterneCass)
            {
                ret=1;
                jeu->joueurs[joueur].action[i]=rien;
            }
        }
        return ret;
    }

    if(action==charPiochRep)
    {
        for(int i=0; i<3; i++)
        {
            if(jeu->joueurs[joueur].action[i]==outilsCass)
            {
                jeu->joueurs[joueur].action[i]=lanterneCass;
                return 1;
            }
            if(jeu->joueurs[joueur].action[i]==charioCass)
            {
                ret=1;
                jeu->joueurs[joueur].action[i]=rien;
            }
            if(jeu->joueurs[joueur].action[i]==piocheCass)
            {
                ret=1;
                jeu->joueurs[joueur].action[i]=rien;
            }
        }
        return ret;
    }
    if(action==piocheCass || action==charioCass || action==lanterneCass)
    {
        int casser=0;
        for(int i=0; i<3; i++)
        {
            typeCarte carteAction=jeu->joueurs[joueur].action[i];
            if(carteAction==piocheCass || carteAction==charioCass || carteAction==lanterneCass)
            {
                casser++;
            }
        }
        if(casser<2)
        {
            for(int i=0; i<3; i++)
            {
                if(jeu->joueurs[joueur].action[i]==rien)
                {
                    jeu->joueurs[joueur].action[i]=action;
                    return 1;
                }
            }
        }
        else
        {
            int placer=0;
            for(int i=0; i<3; i++)
            {
                typeCarte carteAction=jeu->joueurs[joueur].action[i];
                if(carteAction==piocheCass || carteAction==charioCass || carteAction==lanterneCass)
                {
                    if(placer==0)
                    {
                        placer=1;
                        jeu->joueurs[joueur].action[i]=outilsCass;
                    }
                    else
                    {
                        jeu->joueurs[joueur].action[i]=rien;
                    }
                }
            }
            return 1;
        }
    }
    if(action==outilsCass)
    {
        int casser=0;
        for(int i=0; i<3; i++)
        {
            typeCarte carteAction=jeu->joueurs[joueur].action[i];
            if(carteAction==piocheCass || carteAction==charioCass || carteAction==lanterneCass)
            {
                casser++;
            }
        }
        if(casser==0)
        {
            for(int i=0; i<3; i++)
            {
                if(jeu->joueurs[joueur].action[i]==rien)
                {
                    jeu->joueurs[joueur].action[i]=action;
                    return 1;
                }
            }
        }
        else
        {
            int placer=0;
            for(int i=0; i<3; i++)
            {
                typeCarte carteAction=jeu->joueurs[joueur].action[i];
                if(carteAction==piocheCass || carteAction==charioCass || carteAction==lanterneCass)
                {
                    if(placer==0)
                    {
                        placer=1;
                        jeu->joueurs[joueur].action[i]=outilsCass;
                    }
                    else
                    {
                        jeu->joueurs[joueur].action[i]=rien;
                    }
                }
            }
            return 1;
        }
    }
    return 0;
}

int deposerMap(jeu_t *jeu, int taille_x, int taille_y)//ss pgm pour l'utilisation de la carte map
{
    typeCarte *map=NULL;
    if(jeu->defausseNb!=0)//si on commence a defausser, on ne peut pas utiliser la carte map
    {
        return 0;
    }
    for(int i=0; i<6; i++)
    {
        if(jeu->joueurs[jeu->tourJoueur].carte[i]==carteMap)//si le joueur possede une carte map
        {
            map=&(jeu->joueurs[jeu->tourJoueur].carte[i]);
        }
    }
    if(*(jeu->NewCard)==carteMap)//si le joueur a pioché une carte map
    {
        map=jeu->NewCard;
    }
    if(map==NULL)//si le joueur ne possede pas du tout la carte map
    {
        return 0;//renvoi 0
    }
    if(mouse_x<WIDTH-taille_x || mouse_x>WIDTH)//si on ne se place pas dans la colonne des pepites
    {
        return 0;//renvoi 0
    }
    int or=-1;
    if(mouse_y>=HEIGHT-(taille_y*5/1.25)-taille_y/2 && mouse_y<=HEIGHT-(taille_y*5/1.25)-taille_y/2+taille_y)//si on se place sur la premiere carte or
    {
        or=1;
    }
    if(mouse_y>=HEIGHT-(taille_y*5/2)-taille_y/2 && mouse_y<=HEIGHT-(taille_y*5/2)-taille_y/2+taille_y)//si on se place sur la deuxieme carte or
    {
        or=2;
    }
    if(mouse_y>=HEIGHT-(taille_y*5/4.75)-taille_y/2 && mouse_y<=HEIGHT-(taille_y*5/4.75)-taille_y/2+taille_y)//si on se place sur la troisieme carte or
    {
        or=3;
    }
    if(or==-1)
    {
        return 0;
    }
    jeu->selectionner=map;
    jeu->orMap=or;
    return 0;
}

int deposerDefausse(jeu_t *jeu, int taille_x, int taille_y)//ss pgm pour defausser
{
    if(jeu->selectionner==NULL)//si on ne selectionne pas de carte
    {
        return 0;
    }
    if(mouse_x<=WIDTH-taille_x || mouse_x>=WIDTH || mouse_y<=0 || mouse_y>=taille_y)//si on ne se place pas dans les coordonnes de la defausse (haut a droite)
    {
        return 0;
    }
    jeu->defausse=*(jeu->selectionner);//on selectionne la carte qu'on veut defausser
    poserCarte(jeu);
    jeu->defausseNb+=1;//on augmente de 1 le nb de carte defaussé
    if(jeu->defausseNb==3)//si on a defaussé 3 cartes
    {
        return 1;//renvoi 1
    }
    return 2;
}

int poserCarte(jeu_t *jeu)// ss pgm pour poser une carte en fonction des resultats renvoyés par les autres ss pgm
{
    if(jeu->selectionner==jeu->NewCard)//si son selectionne la carte de la pioche
    {
        jeu->NewCard=NULL;
        jeu->selectionner=NULL;
        jeu->orMap=0;
        return 1;
    }
    for(int i=0; i<6; i++)
    {

        if(&(jeu->joueurs[jeu->tourJoueur].carte[i])==jeu->selectionner)//si on selectionne une des cartes de la main du joueur
        {
            jeu->joueurs[jeu->tourJoueur].carte[i]=*(jeu->NewCard);//on la remplace par la carte piochée
            jeu->NewCard=NULL;
            jeu->selectionner=NULL;
            jeu->orMap=0;
            return 1;
        }
    }
}

int dragAndDrop(BITMAP *buffer, jeu_t *jeu, BITMAP *souris, int taille_x, int taille_y)//ss pgm utilisation drag and drop
{
    if(jeu->orMap!=0 && jeu->selectionner!=NULL && *(jeu->selectionner)==carteMap && (mouse_b & 1)==0)//si on selectionne une carte map et qu'on maintien le click gauche
    {
        return poserCarte(jeu);//on apl le ss pgm
    }
     if(mouse_b & 1)//si on fait un click gauche
        {
            if(deposerMap(jeu, taille_x, taille_y)==1)//que le joueur possede une carte map
            {
                return poserCarte(jeu);//on apl le sspgm pour la poser
            }
            return 0;
        }
    else if((mouse_b & 2))//si click droit
    {
        selectionnerCarte(buffer, jeu, taille_x, taille_y);//selection de la carte desiree
    }
    else
    {
        if(deposerCarteChem(buffer, jeu, taille_x, taille_y)==1)//si les conditions sont verifiées pour ce ss pgm
        {
            int res=verifierTunnel(jeu);
            if(res==0)
            {
                return poserCarte(jeu);//on pose la carte
            }
            else
            {
                return res;
            }
        }
        if(deposerCarteAction(buffer, jeu, taille_x, taille_y)==1)//si les conditions sont verifiées pour ce ss pgm
        {
            return poserCarte(jeu);//on peut poser la carte
        }
        int r=deposerDefausse(jeu, taille_x, taille_y);
        if(r!=0)
        {
            return r;
        }
        jeu->selectionner=NULL;
    }
    if(jeu->selectionner!=NULL)
    {
        draw_sprite(buffer, bitmapPourCarte(jeu, *(jeu->selectionner)), mouse_x-taille_x/2, mouse_y-taille_y/2);
    }
    draw_sprite(buffer, souris, mouse_x-17, mouse_y-17); //dessin de la souris
    return 0;
}

void afficherPanneJoueurs(BITMAP *buffer, jeu_t *jeu)//affichage des noms et penalités des joueurs de la partie
{
    int taille_x=75, taille_y=112;
    int depart_x=0, depart_y=taille_y+45;
    rectfill(buffer, 0,taille_y+15, WIDTH, taille_y*2+15, makecol(228,48,215));
    taille_x=40;
    taille_y=60;
    for(int j=0; j<jeu->nbJoueurs; j++)//boucle sur le nb de joueurs
    {
        for(int i=0; i<3; i++)//boucle sur les actions
        {
            if(i==1)
            {
                textprintf_ex(buffer, font, depart_x+(taille_x/2)-5, depart_y-15, makecol(0,0,0), makecol(228,48,215), "%s", jeu->joueurs[j].pseudo);//affichage des pseudos de chaque joueur
            }
            typeCarte carte=jeu->joueurs[j].action[i];
            if(carte!=rien)//si le joueur possede des penalités
            {
                draw_sprite(buffer, bitmapPourMiniCarte(jeu, carte), depart_x, depart_y);//on affiche en petit dans la panne des joueurs
            }
            depart_x+=taille_x;
        }
        depart_x+=20;//on laisse un petit espace entre chaque joueur
    }
}

int afficherPlateau(BITMAP *buffer, jeu_t *jeu, BITMAP *souris)//affichage complet du plateau de jeu
{
    clear_bitmap(buffer);
    int taille_x=75, taille_y=112;
    int depart_x=WIDTH-8*taille_x, depart_y=HEIGHT-5*taille_y;

    afficherMine(jeu, buffer, taille_x, taille_y);//apl des ss pgm
    afficherOr(jeu, buffer, taille_x, taille_y);

    draw_sprite(buffer, bitmapPourCarte(jeu, entreeMine), WIDTH-taille_x*9, HEIGHT-(taille_y*5/2)-taille_y/2);//affichage de l'entree de la mine
    depart_y=HEIGHT-5*taille_y;
    depart_x=20;
    for(int i=0; i<jeu->nbJoueurs; i++)//affichage des pseudos et des scores de joueurs sur le cote
    {
        rectfill(buffer, 0, depart_y, depart_x+100, depart_y+taille_y, makecol(37,255,233));
        textprintf_ex(buffer, font, depart_x, depart_y,makecol(0, 0, 0),makecol(37,255,233), jeu->joueurs[i].pseudo);
        textprintf_ex(buffer, font, depart_x, depart_y+10,makecol(0, 0, 0),makecol(37,255,233), "%d", jeu->joueurs[i].score);
        depart_y+=taille_y+20;
    }
    int bg=makecol(53,53,223);
    rectfill(buffer, 0, 0, WIDTH-taille_x*2, taille_y+15, bg);
    textprintf_ex(buffer, font, 10, 5, makecol(0,0,0), bg, "JOUEUR: %s", jeu->joueurs[jeu->tourJoueur].pseudo);//affichage pseudo et score du joueur en train de jouer
    textprintf_ex(buffer, font, 350, 5, makecol(0,0,0), bg, "SCORE: %d", jeu->joueurs[jeu->tourJoueur].score);
    if(jeu->joueurs[jeu->tourJoueur].role==saboteur)
    {
        textprintf_ex(buffer, font, 450, 5, makecol(0,0,0), bg, "ROLE: SABOTEUR");//affichage du role saboteur
    }
    else
    {
        textprintf_ex(buffer, font, 450, 5, makecol(0,0,0), bg, "ROLE: CHERCHEUR");//affichage du role chercheur
    }
    rectfill(buffer, WIDTH-taille_x*2, 0, WIDTH-taille_x, taille_y, makecol(51,225,82));
    rectfill(buffer, WIDTH-taille_x, 0, WIDTH, taille_y, makecol(51,225,82));
    depart_x=0;
    depart_y=15;
    for(int i=0; i<6; i++)
    {
        if(&(jeu->joueurs[jeu->tourJoueur].carte[i])!=jeu->selectionner)
        {
            draw_sprite(buffer, bitmapPourCarte(jeu, jeu->joueurs[jeu->tourJoueur].carte[i]), depart_x, depart_y);//affichage main du joueur en train de joueur
        }
        depart_x+=taille_x;
    }
    if(jeu->selectionner!=jeu->NewCard)
    {
        draw_sprite(buffer, bitmapPourCarte(jeu, *(jeu->NewCard)), WIDTH-taille_x*2, 0);//affichage pioche
    }
    else
    {
        draw_sprite(buffer, bitmapPourCarte(jeu, vide), WIDTH-taille_x*2, 0);
    }
    if(jeu->defausse!=rien)
    {
       draw_sprite(buffer, bitmapPourCarte(jeu, jeu->defausse), WIDTH-taille_x, 0);//affichage de la defausse
    }

    afficherPanneJoueurs(buffer, jeu);//apl ss pgm
    if(jeu->or1R==1 && jeu->or2R==1 && jeu->or3R==1)
    {
        blit(buffer, screen, 0,0,0,0,WIDTH, HEIGHT);
        Sleep(2000);
        return 21;
    }
    int a=dragAndDrop(buffer, jeu, souris, taille_x, taille_y);

    blit(buffer, screen, 0,0,0,0,WIDTH, HEIGHT);
    return a;
}
