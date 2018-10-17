#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include <SDL/SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "SDL/SDL_ttf.h"
#include "Chargement_image.h"
#include "ttf.h"
#include <iostream>

int main( int argc, char *argv[ ] )
{
/*
//******************************************
//***************** TUTO 1 *****************
//******************************************
//  attribut de l'�cran
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    const int SCREEN_BPP = 32; // nombre de bit par pixel

//Les surfaces que nous allons utiliser
    SDL_Surface *message = NULL; //surface de l'�cran
    SDL_Surface *background = NULL; // fond d'�cran
    SDL_Surface *screen = NULL; //fen�ter ou charger une image

//SDL_Surface *load_image( std::string filename )



//Initialisation de tous les sous-syst�mes de SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return EXIT_FAILURE;
    }
//Mise en place de l'�cran
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

//S'il y a une erreur dans la cr�ation de l'�cran
    if( screen == NULL )
    {
        return EXIT_FAILURE;
    }
//Mise en place de la barre caption
    SDL_WM_SetCaption( "Hello World", NULL );
//Chargement des images
    message = load_image( "hello_world.bmp" );
    background = load_image( "background.bmp" );
//On applique le fond sur l'�cran
    apply_surface( 0, 0, background, screen );

//On applique le message sur l'�cran
    apply_surface( 180, 140, message, screen );

//Mise � jour de l'�cran (sinon sa affiche rien)
    if( SDL_Flip( screen ) == -1 )
    {
        return EXIT_FAILURE;
    }

//Dur�e de l'image
    SDL_Delay( 3000 );

//Lib�ration des surfaces
    SDL_FreeSurface( message );
    SDL_FreeSurface( background );
*/

//******************************************
//***************** TUTO 2 *****************
//******************************************
std::cout << "coucou" << std::endl;
bool toto ;
toto = init();
std::cout << toto << std::endl;
//load_files();



//Dur�e de l'image
    SDL_Delay( 3000 );
//On quitte SDL
    SDL_Quit();

    return 0;
}

