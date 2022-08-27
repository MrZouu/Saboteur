#include "header.h"
void saveScore(char *pseudo, int score)
{
    char *nomFichier;
    FILE *fichier;
    nomFichier=malloc(sizeof(char)*(strlen(pseudo)+7));
    strcpy(nomFichier, "scores/");
    strcat(nomFichier, pseudo);
    fichier=fopen(nomFichier, "w+");
    if(fichier==NULL)
    {
        return;
    }
    fprintf(fichier, "%d", score);
    printf("%s %d\n", nomFichier, score);
    fclose(fichier);
}
int recupScore(char *pseudo)
{
    char *nomFichier;
    FILE *fichier;
    int score=0;
    nomFichier=malloc(sizeof(char)*(strlen(pseudo)+7));
    strcpy(nomFichier, "scores/");
    strcat(nomFichier, pseudo);
    fichier=fopen(nomFichier, "r");
    if(fichier==NULL)
    {
        return 0;
    }
    printf("%s %d\n", nomFichier, score);
    fscanf(fichier, "%d", &score);
    fclose(fichier);
    return score;
}
void creerJoueurs(char **pseudo, int nbJoueur, jeu_t *jeu)//ss pgm pour la creation des joueurs
{
    jeu->nbJoueurs=nbJoueur;
    int sab=rand()%jeu->nbJoueurs;
    for(int i=0; i<jeu->nbJoueurs; i++)
    {
        strcpy(jeu->joueurs[i].pseudo, pseudo[i]);//on assimile les pseudos a chaque joueur
        for(int j=0; j<6; j++)
        {
            jeu->joueurs[i].carte[j]=piocher(jeu);//on fait piocher 6 cartes a chaque joueur(la main)
        }
        if(i==sab)//assimilation du role saboteur a un joueur tiré au hasard
        {
            jeu->joueurs[i].role=saboteur;
        }
        else//les autres sont des chercheurds
        {
            jeu->joueurs[i].role=chercheur;
        }
        jeu->joueurs[i].score=recupScore(pseudo[i]);//initialisation des scores a 0
        for(int j=0 ;j<3;j++)
        {
            jeu->joueurs[i].action[j]=rien;//on initialise les penalités a 0
        }
    }
    jeu->tourJoueur=0;//initialisation des tours a 0
}

int creerNbJoueurs(BITMAP *buffer, BITMAP *souris, BITMAP *image)//ss pgm pour choisir le nombre de joueurs
{
    puts("nbjoueur");
    while(1)
    {
        clear_bitmap(buffer);
        draw_sprite(buffer,image,0,0);
        textprintf_centre(buffer, font, WIDTH/2,HEIGHT/2,makecol(255, 255, 255), "COMBIEN DE JOUEURS PARTICIPENT?");//affichage du texte
        textprintf_centre(buffer, font, WIDTH/3,HEIGHT/1.5,makecol(255, 255, 255), "2 JOUEURS");
        textprintf_centre(buffer, font, WIDTH/2,HEIGHT/1.5,makecol(255, 255, 255), "3 JOUEURS");
        textprintf_centre(buffer, font, WIDTH/1.5,HEIGHT/1.5,makecol(255, 255, 255), "4 JOUEURS");
        draw_sprite(buffer, souris, mouse_x, mouse_y); //dessin de la souris
        blit(buffer, screen, 0,0,0,0,WIDTH, HEIGHT);
        if((mouse_b & 1))//click gauche
        {
            if(mouse_x>=WIDTH/3.2 && mouse_x<=WIDTH/2.8 && mouse_y>=HEIGHT/1.6 && mouse_y<=HEIGHT/1.4)//si le click de la souris se fait dans ces coordonnees
            {
                clear_bitmap(buffer);
                blit(buffer, screen, 0,0,0,0,WIDTH, HEIGHT);
                return 2;// il y a 2 joueurs
            }
            if(mouse_x>=WIDTH/2.1 && mouse_x<=WIDTH/1.9 && mouse_y>=HEIGHT/1.6 && mouse_y<=HEIGHT/1.4)//si le click de la souris se fait dans ces coordonnees
            {
                clear_bitmap(buffer);
                blit(buffer, screen, 0,0,0,0,WIDTH, HEIGHT);
                return 3;//il y a 3 joueurs
            }
            if(mouse_x>=WIDTH/1.6 && mouse_x<=WIDTH/1.4 && mouse_y>=HEIGHT/1.6 && mouse_y<=HEIGHT/1.4)//si le click de la souris se fait dans ces coordonnees
            {
                clear_bitmap(buffer);
                blit(buffer, screen, 0,0,0,0,WIDTH, HEIGHT);
                return 4;//il y a 4 joueurs
            }
        }
    }
}

char *nomJoueurs(BITMAP *buffer, int nombreJoueurs, BITMAP *image)//ss pgm pour choisir le nom des joueurs
{
        puts("nomjoueur");
    int i=0;
    char ascii=0;
    char *pseudo=malloc(sizeof(char)*25);
    if(pseudo==NULL)
    {
        exit(0);
    }
    memset(pseudo, 0, 25);//pseudos vides
    while(1)
    {
        clear_bitmap(buffer);
        draw_sprite(buffer, image ,0,0);
        textprintf_centre(buffer, font, WIDTH/2,HEIGHT/2,makecol(255, 255, 255), "joueur %d, tapez votre pseudo en MINUSCULES, puis appuez sur entree pour valider", nombreJoueurs);
        textprintf_centre(buffer, font, WIDTH/2,HEIGHT/1.5, makecol(255,255,255), "%s", pseudo);
        if(keypressed())
        {
            int keyp=readkey();
             printf("%d\n", keyp);
            ascii=keyp&0xff;//transforme un keyp en char
            char scancode=keyp>>8;
            printf("%d\n", scancode);
            if(ascii>='a' && ascii<='z')//on prend seulement des minuscules
            {
                pseudo[i]=ascii;
                i++;
                pseudo[i]=0;
            }
            else if(scancode==KEY_ENTER)//si touche espace appuyee
            {
                puts("space");
                clear_bitmap(buffer);
                blit(buffer, screen, 0,0,0,0,WIDTH, HEIGHT);
                return pseudo;// on renvoie les pseudos au programme
            }
            else if(scancode==KEY_BACKSPACE)//si touche delete appuyee
            {
                if(i>0)
                {
                    i--;//supprimer caractere
                    pseudo[i]=0;
                }
            }
        }
        blit(buffer, screen, 0,0,0,0,WIDTH, HEIGHT);
    }
}

joueur_t **recupScores()//lire les fichiers score
{
    DIR *d;//comme FILE* mais pour directory
    struct dirent *dir;//structure qui contient les infos d'un fichier
    int n=0;
    joueur_t **res;
    d=opendir("./scores");//ouverture dossier score
    if(d!=NULL)
    {
        while((dir=readdir(d))!=NULL)//on lit le dossier et ca renvoit un fichier
        {
            if(strcmp(dir->d_name,".")==0 || strcmp(dir->d_name, "..")==0)//ignore . et .. qui sont fichiers courant et parant
            {
                continue;
            }
            n++;//compte le nb de fichiers
        }
        closedir(d);
    }
    res=malloc(sizeof(joueur_t*)*n+1);
    int i=0;
    d=opendir("./scores");
    if(d!=NULL)
    {
        while((dir=readdir(d))!=NULL)
        {
            if(strcmp(dir->d_name,".")==0 || strcmp(dir->d_name, "..")==0)
            {
                continue;
            }
            res[i]=malloc(sizeof(joueur_t));
            strcpy(res[i]->pseudo, dir->d_name);//on copie le nom du fichier dans le plseudo du joueur
            res[i]->score=recupScore(res[i]->pseudo);//on recupere le score qui correspond au pseudo
            i++;
        }
        res[i]=NULL;
        closedir(d);
    }
    return res;
}
