#include "header.h"
void animationWinSabo(BITMAP *buffer)
{
    BITMAP* mine;
    BITMAP* fond;
   // BITMAP* chercheur;
    BITMAP* saboteur;
    BITMAP* pepite;
    t_chercheur chercheurMvt;
    srand(time(NULL));
    int x=0,y=0,xp=0,yp=0;
    //int cx=WIDTH/1.7,cy=HEIGHT/1.8; //chercheur coordonnees
    int sx=30, sy=HEIGHT/1.35; //Saboteur coordonnees
    int scancode=0,touche=0;
    int posX=0,posY=0;
    chercheurMvt.posLig = HEIGHT/1.75;     chercheurMvt.depLig = 1;
    chercheurMvt.posCol = WIDTH/1.5;     chercheurMvt.depCol = 1;




    //On charge les images
    mine=load_bitmap("assets/mine2.bmp",NULL);
    fond=load_bitmap("assets/fond.bmp",NULL);
    chercheurMvt.chercheur=load_bitmap("assets/chercheur.bmp",NULL);
    saboteur=load_bitmap("assets/saboteur.bmp",NULL);
    pepite=load_bitmap("assets/pepite.bmp",NULL);

    blit(buffer,screen,0,0,0,0,WIDTH,HEIGHT);
    //Affichage saboteur et chercheur
    masked_blit(saboteur,screen,0,0,30,WIDTH/1.35,WIDTH,HEIGHT);
    masked_blit(chercheurMvt.chercheur,screen,0,0,0,0,WIDTH,HEIGHT);

    int px, py; //coordonnees pepite
    px = sx;//Memes coordonnees que le saboteur a l'origine
    py = sy;
    int compteur=0;
    while(!key[KEY_ESC])
    {
        clear_bitmap(buffer);

        px++;//pépite avance
        compteur++;
        if(px>WIDTH)
        {
            px=sx;//pépite retourne a saboteur si pépite dépasse de l'écran
            compteur=0;
            py=sy;
        }
        if(compteur<385)
        {
            py-=3*(1-(compteur*0.005));
        }
        else
        {
            py+=1+(compteur*0.003);
        }
        draw_sprite(buffer,fond,xp-200,yp);
        draw_sprite(buffer,mine,xp,yp+222);
        draw_sprite(buffer,chercheurMvt.chercheur,chercheurMvt.posCol,chercheurMvt.posLig);//Position chercheur en mouvement
        draw_sprite(buffer,saboteur,sx,sy);
        draw_sprite(buffer,pepite,px,py);


        textprintf_ex(buffer,font,10,10,makecol(255,255,255),-1,"VICTOIRE DU SABOTEUR");//Affichage angle
        textprintf_ex(buffer,font,10,20,makecol(255,255,255),-1,"PRESSER ESPACE POUR QUITTER");
        blit(buffer,screen,0,0,0,0,WIDTH,HEIGHT);
        //Code inspiré exercice 4 tp 2
        chercheurMvt.posCol = chercheurMvt.posCol+ chercheurMvt.depCol; //inspiré par ex2 seance2 cours
        chercheurMvt.posLig = chercheurMvt.posLig + chercheurMvt.depLig;
        if (chercheurMvt.posLig >=HEIGHT/1.25 || chercheurMvt.posLig<=HEIGHT/2.5)
         {
            chercheurMvt.depLig = chercheurMvt.depLig *(-1);
         }
        if (chercheurMvt.posCol>=WIDTH/2)
        {
            chercheurMvt.depCol = chercheurMvt.depCol *(-1);
        }
        if (chercheurMvt.posLig<0)
        {
            chercheurMvt.depLig = chercheurMvt.depLig *(-1);
        }
        if (chercheurMvt.posCol<WIDTH-100)
        {
            chercheurMvt.depCol = chercheurMvt.depCol *(-1);
        }
        if(keypressed())
        {
            touche=readkey();
            scancode=touche>>8;

            switch (scancode)
            {
            case KEY_LEFT ://Si flèche gauche
                if(xp<=195)
                {
                    xp+=10;//Avance le fond de 10
                }
                break;
            case KEY_RIGHT ://Si flèche droite
                if(xp>=-195)
                {
                    xp-=10;//Recule le don de 10
                }
                break;
            }
        }
    }
}

void ecranWinSaboteurs(BITMAP *buffer)//ecran de victoire saboteur
{
    int x=0;
    BITMAP *gameover;
    while(!key[KEY_ESC])
    {

        clear_bitmap(buffer);
        gameover=load_bitmap("assets/ecrangameover.bmp", NULL);
        if(!gameover)
        {
            puts("erreur!");
        }
        stretch_sprite(buffer, gameover, 0,0,WIDTH, HEIGHT);

        if(key[KEY_SPACE])
        {
             break;

        }
        blit(buffer, screen, 0,0,0,0, WIDTH, HEIGHT);
    }
                animationWinSabo(buffer);

    return;
}
void ecranWinChercheurs(BITMAP *buffer)//ecran ed victoire chercheurs
{
    srand(time(NULL));
    BITMAP *chariot;
    BITMAP *mine;
    BITMAP *pepite;
    BITMAP *chercheur;
    BITMAP *fond;
    int pepite_x[10];
    int pepite_y[10];
    clear_bitmap(buffer);
    chercheur=load_bitmap("assets/chercheur.bmp", NULL);
    chariot=load_bitmap("assets/chariot.bmp", NULL);
    pepite=load_bitmap("assets/pepite.bmp", NULL);
    mine=load_bitmap("assets/mine.bmp", NULL);
    fond=load_bitmap("assets/fond.bmp", NULL);
    BITMAP *texte=load_bitmap("assets/textewin.bmp", NULL);
    if (!mine || !chariot || !pepite)
    {
        allegro_message("pas pu trouver saboteur.bmp");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<10; i++)
    {
        pepite_x[i]=WIDTH/2+rand()%100-50;
        pepite_y[i]=HEIGHT/1.5;
    }
    while(!key[KEY_ESC])
    {
        clear_bitmap(buffer);
        draw_sprite(buffer, fond, 0,0);

        stretch_sprite(buffer, mine, 0,0, WIDTH, HEIGHT);//affichage image de la mine a la taille de l'ecran
        draw_sprite(buffer, texte, 20, HEIGHT/3.5);
        ///les 10 lignes suivantes ont été réalisées avec l'aide de Lancelot De Montfort
        for(int i=0; i<10; i++)
        {
            stretch_sprite(buffer, pepite, pepite_x[i], pepite_y[i], 30, 30);//affichage des pepites
            pepite_y[i]--;//permet de bouger la pepite sur l'axe Y
            if(pepite_y[i]<-30)//si la pepite sors de l'ecran
            {
                pepite_x[i]=WIDTH/2+rand()%100-50;//on change ses coordonness
                pepite_y[i]=HEIGHT/1.5+rand()%100-50;
            }
        }
        draw_sprite(buffer, chercheur, WIDTH/4, HEIGHT/2);
        draw_sprite_h_flip(buffer, chercheur, WIDTH/1.75, HEIGHT/2);
        draw_sprite(buffer, chariot, WIDTH/2.65, HEIGHT/1.75);
        blit(buffer,screen,0,0,0,0,WIDTH,HEIGHT);

    }
    return;
}
