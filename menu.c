#include "header.h"

void ecranScores(BITMAP *buffer, joueur_t **scores)
{
    int x=50, y=50;
    clear_bitmap(buffer);
        for(int i=0; scores[i]!=NULL; i++)
        {
            puts(scores[i]->pseudo);
            textprintf_centre(buffer, font, x,y,makecol(255,0,0), "%s: %d", scores[i]->pseudo, scores[i]->score);
            y+=20;
        }
        blit(buffer, screen, 0,0,0,0, WIDTH, HEIGHT);
    while(!key[KEY_ESC])
    {

    }
}
void menu(BITMAP *buffer, BITMAP *souris)
{
    int gris;
    int blanc;
    int noir;
    int aide=0;
    char **pseudos;
    int nb;
    BITMAP * image;
    BITMAP * logo;
    image = load_bitmap("assets/image.bmp",NULL); //Charger image mine.bmp
    logo = load_bitmap("assets/logo.bmp",NULL); //charger image logo.bmp
    // Avoir une couleur (ici du gris, blanc, noir)
    gris=makecol(233,211,58);
    blanc=makecol(234,234,234);
    noir=makecol(0,0,0);
    //images trouvés sur google image

    while(!key[KEY_ESC])
    {
        clear_bitmap(buffer);
        draw_sprite(buffer, image, 0,0); //Affichage image fond
        // afficher du texte coin supérieur gauche

        //Position haut gauche//Hauteur angle haut gauche//Position horizontale bas droit//Hauteur angle bas droit
        rectfill(buffer,195,790,5,700,gris);
        textprintf_ex(buffer,font,37,740,noir,-1,"NOUVELLE PARTIE");
        rectfill(buffer,395,790,205,700,gris);
        textprintf_ex(buffer,font,233,740,noir,-1,"PARTIE SAUVEGARDEE");
        rectfill(buffer,595,790,405,700,gris);
        textprintf_ex(buffer,font,445,740,noir,-1,"SCORES JOUEURS");
        rectfill(buffer,795,790,605,700,gris);
        textprintf_ex(buffer,font,645,740,noir,-1,"QUITTER LE JEU");
        if(aide==0)
        {
            draw_sprite(buffer,logo,0,0); //Affichage image logo sur le fond
            rectfill(buffer,795,10,605,90,gris); //rectangle haut droit
            textprintf_ex(buffer,font,685,45,noir,-1,"AIDE");
        }
        if(aide==1)
        {
            //Regles du jeu dans le menu aide
                textprintf_ex(buffer,font,10,10,blanc,-1,"LE SABOTEUR");
                textprintf_ex(buffer,font,10,30,blanc,-1,"REGLES DU JEU :");
                textprintf_ex(buffer,font,10,40,blanc,-1,"Saboteur est un jeu de cartes dans lequel les joueurs jouent le role secret");
                textprintf_ex(buffer,font,10,50,blanc,-1,"de chercheur d or ou du saboteur qui souhaite entraver l exploration.");

                textprintf_ex(buffer,font,10,70,blanc,-1,"MINEUR");
                textprintf_ex(buffer,font,10,80,blanc,-1,"Les mineurs doivent essayer d obtenir le plus grand nombre possible de pepites d or");
                textprintf_ex(buffer,font,10,90,blanc,-1,"au cours des trois rounds. Du jeu.");

                textprintf_ex(buffer,font,10,110,blanc,-1,"SABOTEUR");
                textprintf_ex(buffer,font,10,120,blanc,-1,"Quand vous etes saboteur, vous devez empecher les mineurs d avoir le tresor.");
                textprintf_ex(buffer,font,10,130,blanc,-1,"Si les chercheurs d or n atteignent pas le tresor, le saboteur gagnera les pepites");

                textprintf_ex(buffer,font,10,150,blanc,-1,"Vous devez placer des cartes de votre main, avec une carte chemin,");
                textprintf_ex(buffer,font,10,160,blanc,-1,"une carte action ( sabotage, reparation, chute de pierre ), Ou passer.");
                textprintf_ex(buffer,font,10,170,blanc,-1,"Les cartes chemins doivent etre placees pour creer un chemin ininterrompu, du depart au but");
                textprintf_ex(buffer,font,10,190,blanc,-1,"Le joueur avec le plus de pepites d or apres le troisieme tour gagne.");

                //On garde affiché les options du menu
                rectfill(buffer,195,790,5,700,gris);
                textprintf_ex(buffer,font,37,740,noir,-1,"NOUVELLE PARTIE");
                rectfill(buffer,395,790,205,700,gris);
                textprintf_ex(buffer,font,233,740,noir,-1,"PARTIE SAUVEGARDEE");
                rectfill(buffer,595,790,405,700,gris);
                textprintf_ex(buffer,font,445,740,noir,-1,"SCORES JOUEURS");
                rectfill(buffer,795,790,605,700,gris);
                textprintf_ex(buffer,font,645,740,noir,-1,"QUITTER LE JEU");
        }
        draw_sprite(buffer, souris, mouse_x, mouse_y);
        blit(buffer, screen, 0,0,0,0,WIDTH, HEIGHT);

        if((mouse_b&1))
        {

            if(mouse_x>=WIDTH/80 && mouse_x<=WIDTH/4 && mouse_y>=HEIGHT/1.15 && mouse_y<=HEIGHT)
            {
                nb=creerNbJoueurs(buffer, souris, image);//on recupere le nombre de joueurs
                pseudos=malloc(sizeof(char*)*nb);
                if(pseudos==NULL)
                {
                    exit(0);
                }
                for(int i=0; i<nb; i++)
                {
                    pseudos[i]=nomJoueurs(buffer, i+1, image);//on attribue a chaque joueur son pseudo
                }
                debutPartie(pseudos, nb, souris);
            }
            if(mouse_x>=WIDTH/2 && mouse_x<=WIDTH/1.35 && mouse_y>=HEIGHT/1.15 && mouse_y<=HEIGHT)
            {
                joueur_t **scores=recupScores();
                ecranScores(buffer, scores);
            }
            if(mouse_x>=WIDTH/1.35 && mouse_x<=WIDTH && mouse_y>=HEIGHT/1.15 && mouse_y<=HEIGHT)
            {
                destroy_bitmap(buffer);
                allegro_exit(); //On quitte le jeu
            }

            if(mouse_x>=WIDTH/1.35 && mouse_x<=WIDTH && mouse_y>=HEIGHT/80 && mouse_y<=HEIGHT/7.5) //Menu aide
            {
                aide=1;
            }
            Sleep(100);
        }
    }
    readkey();
}
