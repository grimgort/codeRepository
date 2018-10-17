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
//  attribut de l'écran
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    const int SCREEN_BPP = 32; // nombre de bit par pixel

//Les surfaces que nous allons utiliser
    SDL_Surface *message = NULL; //surface de l'écran
    SDL_Surface *background = NULL; // fond d'écran
    SDL_Surface *screen = NULL; //fenêter ou charger une image

//SDL_Surface *load_image( std::string filename )



//Initialisation de tous les sous-systèmes de SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return EXIT_FAILURE;
    }
//Mise en place de l'écran
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

//S'il y a une erreur dans la création de l'écran
    if( screen == NULL )
    {
        return EXIT_FAILURE;
    }
//Mise en place de la barre caption
    SDL_WM_SetCaption( "Hello World", NULL );
//Chargement des images
    message = load_image( "hello_world.bmp" );
    background = load_image( "background.bmp" );
//On applique le fond sur l'écran
    apply_surface( 0, 0, background, screen );

//On applique le message sur l'écran
    apply_surface( 180, 140, message, screen );

//Mise à jour de l'écran (sinon sa affiche rien)
    if( SDL_Flip( screen ) == -1 )
    {
        return EXIT_FAILURE;
    }

//Durée de l'image
    SDL_Delay( 3000 );

//Libération des surfaces
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



//Durée de l'image
    SDL_Delay( 3000 );
//On quitte SDL
    SDL_Quit();

    return 0;
}

