/*******************************************************/
/** Programme de PERRUCHON Romain pour developpez.com **/
/**            Chapitre II : Hello World              **/
/*******************************************************/

//Les fichiers d'entete
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>


#include "fonction_basic.h"
#include "Bitmap.h"
#include "gravite.h"

//Les attributs de l'ecran (640 * 480)
const int SCREEN_WIDTH = 1285;
const int SCREEN_HEIGHT = 697;
const int SCREEN_BPP = 32;

const int TUX_WIDTH = 32;
const int TUX_HEIGHT = 32;
SDL_Color textColor = { 255, 255, 255 };
std::stringstream s("");
//Le nombre de frame par seconde
const int FRAMES_PER_SECOND = 20;
//Les surfaces
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *message = NULL ;
SDL_Surface *message2 = NULL ;
SDL_Surface *ciel = NULL ;
SDL_Surface *tux = NULL ;

//La structure d'evenement
SDL_Event event;

TTF_Font *font;

class Tux
{
private:
    //Les coordonnées x et y du tux
    int x, y;

    //La vitesse du tux
    int xVel, yVel;

    double Vx0, Vz0, masse, deltaX, deltaY;

    std::vector<int> coordCollisionX;
    std::vector<int> coordCollisionY;

public:
    //Initialisation des variables
    Tux();

    //Recupere la touche pressee et ajuste la vitesse du tux
    void handle_input();

    //Montre le tux sur l'ecran
    void show();

    //Boite de collision
    void collision();

    void X1(double Fx0, double deltaT);
    void Z1(double Fz0, double deltaT);
};

Tux::Tux()
{
    //Initialisation des coordonnees
    x = 0;
    y = 0;
    Vx0=0;
    Vz0=0;
    masse= 70.0;
    deltaX = 0 ;
    deltaY = 0 ;

    //Initialisation de la vitesse
    xVel = 0;
    yVel = 0;
}

/*
X1 = numero de pixel en x à t+1
Fx0 = Force en x à t
Vx0 = vitesse en x à t
masse = masse de l'objet
Vx1 = vitesse en x à t+1
masse = masse de l'objet
*/
void Tux::X1(double Fx0, double deltaT)
{
    double Vx1 = Vx0 + Fx0/masse*deltaT ;
    Vx0 = Vx1 ;
    deltaX = ceil(deltaT * Vx1);
    x = x + deltaX ;
    for (int i=0; i<=coordCollisionX.size(); i++)
    {
        coordCollisionX[i]=coordCollisionX[i]+deltaX;
    }
}

/*
Z1 = numero de pixel en z à t+1
Fz0 = Force en z à t
Vz1 = vitesse en z à t+1
Vz0 = vitesse en z à t
masse = masse de l'objet
*/
void Tux::Z1(double Fz0, double deltaT)
{
    double Vz1 = Vz0 + Fz0/masse*deltaT ;
    Vz0=Vz1;
    deltaY = ceil(deltaT * Vz1) ;
    y = y + deltaY ;
    for (int i=0; i<=coordCollisionX.size(); i++)
    {
        coordCollisionY[i]=coordCollisionY[i]+deltaY;
    }
}

void Tux::collision()
{
    for (int i=x-50; i<x+50; i++)
    {
        if (i<SCREEN_WIDTH-1 && i>0)
        {

            for (int j=y-50; j<y+50; j++)
            {
                if (j<SCREEN_HEIGHT-1 && j>0)
                {
                    if (obtenirPixel(tux,i,j)==SDL_MapRGB( background->format, 0, 0xFF, 0xFF )  )
                    {
                        coordCollisionX.push_back(i);
                        coordCollisionY.push_back(j);
                    }
                }
            }
        }
    }



}

void Tux::handle_input()
{
    //Si une touche a ete pressee
    if( event.type == SDL_KEYDOWN )
    {
        //ajustement de la vitesse
        switch( event.key.keysym.sym )
        {
        case SDLK_UP:
            yVel -= TUX_HEIGHT / 2;


            break;
        case SDLK_DOWN:
            yVel += TUX_HEIGHT / 2;

            break;
        case SDLK_LEFT:
            xVel -= TUX_WIDTH / 2;

            break;
        case SDLK_RIGHT:
            xVel += TUX_WIDTH / 2;
            break;
        default:
            break;
        }
    }
    //Si une touche a ete relachee
    else if( event.type == SDL_KEYUP )
    {
        //ajustement de la vitesse
        switch( event.key.keysym.sym )
        {
        case SDLK_UP:
            yVel += TUX_HEIGHT / 2;
            break;
        case SDLK_DOWN:
            yVel -= TUX_HEIGHT / 2;
            break;
        case SDLK_LEFT:
            xVel += TUX_WIDTH / 2;
            break;
        case SDLK_RIGHT:
            xVel -= TUX_WIDTH / 2;
            break;
        default:
            break;
        }
    }
}

void Tux::show()
{
    //Bouge le tux à gauche ou à droite
    x += xVel;
    for (int i=0; i<=coordCollisionX.size(); i++)
    {
        coordCollisionX[i]=coordCollisionX[i]+xVel;
    }
    //Si le tux se rapproche trop des limites(gauche ou droite) de l'ecran
    if( ( x < 0 ) || ( x + TUX_WIDTH > SCREEN_WIDTH ) )
    {
        //On revient
        x -= xVel;
        for (int i=0; i<=coordCollisionX.size(); i++)
        {
            coordCollisionX[i]=coordCollisionX[i]-xVel;
        }
        //On revient
        x -= deltaX;
        for (int i=0; i<=coordCollisionX.size(); i++)
        {
            coordCollisionX[i]=coordCollisionX[i]-deltaX;
        }
    }

    //Bouge le tux en haut ou en bas
    y += yVel;
    for (int i=0; i<=coordCollisionY.size(); i++)
    {
        coordCollisionY[i]=coordCollisionY[i]+yVel;
    }
    //Si le tux se rapproche trop des limites(haute ou basse) de l'ecran
    if( ( y < 0 ) || ( y + TUX_HEIGHT > SCREEN_HEIGHT ) )
    {
        //On revient
        y -= yVel;
        for (int i=0; i<=coordCollisionY.size(); i++)
        {
            coordCollisionY[i]=coordCollisionY[i]-yVel;
        }
        //On revient
        y -= deltaY;
        for (int i=0; i<=coordCollisionY.size(); i++)
        {
            coordCollisionY[i]=coordCollisionY[i]-deltaY;
        }
    }

    //Détection des collisions avancé

    s.str("");
    for (int i=0; i<coordCollisionY.size(); i++)
    {
        for (int j=0; j<coordCollisionX.size(); j++)
        {
            if (obtenirPixel(background,coordCollisionX[j],coordCollisionY[i])!=SDL_MapRGB( background->format, 255, 0, 255 ))
            {
                s.str("");
                s << "BOUM " ;
                i=coordCollisionY.size();   // sort de la boucle i
                j=coordCollisionX.size();   // sort de la boucle j
                //On revient
                y -= yVel;
                x -= xVel;
                for (int i=0; i<=coordCollisionY.size(); i++)
                {
                    coordCollisionY[i]=coordCollisionY[i]-yVel;
                }
                for (int j=0; j<=coordCollisionX.size(); j++)
                {
                    coordCollisionX[j]=coordCollisionX[j]-xVel;
                }
                y -= deltaY;
                for (int i=0; i<=coordCollisionY.size(); i++)
                {
                    coordCollisionY[i]=coordCollisionY[i]-deltaY;
                }
                x -= deltaX;
                for (int i=0; i<=coordCollisionX.size(); i++)
                {
                    coordCollisionX[i]=coordCollisionX[i]-deltaX;
                }
            }
// MISE EN PLACE DE LA GRAVITE : Il faut limiter ajouter la variable temps pour avoir une chutte progressive
//                else
//                {
//                    y += 1; //vitesse de  chutte
//                    for (int i=0; i<=coordCollisionY.size(); i++)
//                    {
//                        coordCollisionY[i]=coordCollisionY[i]+1;
//                    }
//                }
        }
    }

    apply_surface( 0, 0, background, screen );
    //Affiche le tux
    apply_surface( x, y, tux, screen );
    message2 = TTF_RenderText_Solid(font, s.str().c_str(), textColor); //crer l'image du texte. Le texte doit être un string complété d'un c_str() pour le transformer en const *char

    apply_surface( 50, 50, message2, screen ); // applique le message a l'écran
}







bool init()
{
    //Initialisation de tous les sous-système de SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Initialisation de SDL_TTF
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Mise en place de l'écran
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //S'il y a eu une erreur lors de la mise en place de l'écran
    if( screen == NULL )
    {
        return false;
    }

    //Mise en place de la barre caption
    SDL_WM_SetCaption( "Destruction", NULL );

    //Ouverture du Font
    font = TTF_OpenFont( "CaslonBold.ttf", 28 );

    //S'il y a une erreur dans le chargement du Font
    if( font == NULL )
    {
        return false;
    }

    //Si tout s'est bien passé
    return true;
}
//Le timer
class Timer
{
private:
    //Le temps quand le timer est lancé
    int startTicks;

    //Les "ticks" enregistré quand le Timer a été mit en pause
    int pausedTicks;

    //Le status du Timer
    bool paused;
    bool started;

public:
    //Initialise les variables (le constructeur)
    Timer();

    //Les différentes actions du timer
    void start();
    void stop();
    void pause();
    void unpause();

    //recupére le nombre de ticks depuis que le timer a été lancé
    //ou récupére le nombre de ticks depuis que le timer a été mis en pause
    int get_ticks();

    //Fonctions de vérification du status du timer
    bool is_started();
    bool is_paused();
};

Timer::Timer()
{
    //Initialisation des variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //On demarre le timer
    started = true;

    //On enlève la pause du timer
    paused = false;

    //On récupére le temps courant
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //On stoppe le timer
    started = false;

    //On enlève la pause
    paused = false;
}

void Timer::pause()
{
    //Si le timer est en marche et pas encore en pause
    if( ( started == true ) && ( paused == false ) )
    {
        //On met en pause le timer
        paused = true;

        //On calcul le pausedTicks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //Si le timer est en pause
    if( paused == true )
    {
        //on enlève la pause du timer
        paused = false;

        //On remet à zero le startTicks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Remise à zero du pausedTicks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //Si le timer est en marche
    if( started == true )
    {
        //Si le timer est en pause
        if( paused == true )
        {
            //On retourne le nombre de ticks quand le timer a été mis en pause
            return pausedTicks;
        }
        else
        {
            //On retourne le temps courant moins le temps quand il a démarré
            return SDL_GetTicks() - startTicks;
        }
    }

    //Si le timer n'est pas en marche
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}




int main( int argc, char* args[] )
{

    bool quit = false;
    int erreur(0);
    //Initialisation
    if( init() == false )
    {
        return 1;
    }


    //Chargement des images
    background = load_image( "more-trees.png" );
    ciel = load_image( "ciel.png" );
    tux = load_image2( "tux-sit.png" );
    //Le tux qu'on va utilise
    Tux monTux;
    //Le regulateur
    Timer fps;
//    Timer loiMeca;
    //Application des surfaces sur l'ecran
    apply_surface( 0, 0, ciel, screen );
    apply_surface( 0, 0, background, screen );

    erreur = updateScreen(screen);

//Uint32 pixel ;
//    pixel = get_pixel32( 5, 6, background );

    Uint32 bgColor = SDL_MapRGB( background->format, 255, 0, 255 ); //Correspond a la couleur rose
    //on met tout les pixel de couleur R 0, G 0xFF, B 0xFF transparent
//            SDL_SetColorKey( background, SDL_RLEACCEL | SDL_SRCCOLORKEY, bgColor );
    //Application des surfaces sur l'ecran
//    apply_surface( 0, 0, background, screen );

    monTux.collision();




    double Fx0(0),Fz0(0), temps;
    int xm, ym; // Position de la souris variable

    int xr(50),yr(50);  //rayon de l'impact du clic souris
    Uint32 pixelTarget;
//    std::stringstream s;
//loiMeca.start();
    //Tant que l'utilisateur n'a pas quitter
    while( quit == false )
    {
        //tant qu'il y a un evenement dans le handler
        //On demarre le timer fps
        fps.start();

        while( SDL_PollEvent( &event ) )
        {
            /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            %%%%%%%%%%% Afficher position de la souris%%%%%%%%%%%%%%%%%
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

            SDL_GetMouseState(&xm, &ym) ;   //donne la position de la souris à partir du coin gauche/haut de la fenêtre
//            if (xm<SCREEN_WIDTH-1 && xm > 0 && ym<SCREEN_HEIGHT-1 && ym >0) //Vérifie que la souris est compris dans l'écran (évite les plantage)
//            {
//                Uint32 couleurSouris = obtenirPixel(background,xm,ym);
//                s.str("");
//                s << "Mouse xpos: " << xm << " Mouse ypos: " << ym << " Couleur: "<< couleurSouris;
//                message = TTF_RenderText_Solid(font, s.str().c_str(), textColor); //crer l'image du texte. Le texte doit être un string complété d'un c_str() pour le transformer en const *char
//                apply_surface( 10, 10, message, screen ); // applique le message a l'écran
//            }


            switch(event.type)
            {

            case SDL_MOUSEBUTTONDOWN:   //Evenement quand on clic sur la souris
            {

                message = TTF_RenderText_Solid(font, "Clic de la souris", textColor); //On précise que l'on a cliké sur la souris
                apply_surface( 10, 35, message, screen );
                s.str("");
                s << "X: " << xm-xr << " Y: " << ym-yr;
                message = TTF_RenderText_Solid(font, s.str().c_str(), textColor); //crer l'image du texte. Le texte doit être un string complété d'un c_str() pour le transformer en const *char
                apply_surface( 10, 70, message, screen );
                for (int i=xm-xr; i<xm+xr; i++)
                {
                    if (i<SCREEN_WIDTH-1 && i>0)
                    {

                        for (int j=ym-yr; j<ym+yr; j++)
                        {
                            if (j<SCREEN_HEIGHT-1 && j>0)
                            {
                                //                        pixelTarget=get_pixel32(i,j,background);
                                definirPixel(background,i,j,bgColor);
                            }
                        }
                    }
                }
            }


            }




            //On recupere l'evenement pour le tux
            monTux.handle_input();


            //Si l'utilisateur ferme la fenetre avec le X
            if( event.type == SDL_QUIT )
            {
                //On quitte le programme
                quit = true;
            }
        }


        // A placer en fin d'événement pour prendre en compte tous les événement
        erreur = updateScreen(screen); //Actualise l'écran pour afficher le message
        apply_surface( 0, 0, ciel, screen );
        apply_surface( 0, 0, background, screen );  //application de la surface à l'écran (permet d'effacer l'ancien message (TTF_RenderText_Solid))
        s.str("");
        s << "fps.get_ticks()" <<fps.get_ticks();
        message = TTF_RenderText_Solid(font, s.str().c_str(), textColor); //crer l'image du texte. Le texte doit être un string complété d'un c_str() pour le transformer en const *char
        apply_surface( 10, 10, message, screen ); // applique le message a l'écran        //On affiche le tux sur l'ecran

        //Tant que le timer fps n'est pas assez haut
        while( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            //On attend...
        }

//         if (fps.get_ticks()>=FRAMES_PER_SECOND)
//        {
        Fx0=0.0;
        Fz0=9.8*70;

        temps = static_cast<double>(fps.get_ticks())/1000. ; //temps en secondes
        monTux.X1(Fx0,temps);
        monTux.Z1(Fz0,temps);
//        }

        monTux.show();



    }
    clean_up(background, message, font);

}
