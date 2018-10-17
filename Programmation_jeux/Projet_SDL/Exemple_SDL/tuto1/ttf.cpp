#include <SDL/SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "SDL/SDL_ttf.h"
#include "Chargement_image.h"


bool init() {
	//Initialisation de tous les sous-syst�mes de SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
		return false;
	}
//  attribut de l'�cran
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    const int SCREEN_BPP = 32; // nombre de bit par pixel

//Les surfaces que nous allons utiliser
    SDL_Surface *message = NULL; //surface de l'�cran
    SDL_Surface *background = NULL; // fond d'�cran
    SDL_Surface *screen = NULL; //fen�ter ou charger une image
	//Mise en place de l'�cran
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

	//S'il y a une erreur dans la mise en place de l'�cran
	if( screen == NULL ) {
		return false;
	}

	//Initialisation de SDL_TTF
	if( TTF_Init() == -1 ) {
		return false;
	}

	//Mise en place de la barre caption
	SDL_WM_SetCaption( "TTF Test", NULL );

//La structure d'�v�nement
SDL_Event event;

//Le Font qu'on va utiliser
TTF_Font *font;

//La couleur du Font
SDL_Color textColor = { 255, 255, 255 };

	//Chargement de l'image de fond
	background = load_image( "background.png" ); // utilisation de la librairie SDL_image

	//Ouverture du Font
	font = TTF_OpenFont( "CaslonBold.ttf", 28 );

	//S'il y a un probl�me au chargement du fond
	if( background == NULL ) {
		return false;
	}

	//S'il y a une erreur dans le chargement du Font
	if( font == NULL ) {
		return false;
	}

//Mise en place du texte sur la surface message
message = TTF_RenderText_Solid( font, "Test pour sdl_ttf", textColor );

//S'il y a une erreur dans la mise en place du texte
if( message == NULL ) {
	return 1;
}

//Application des images sur l'�cran
apply_surface( 0, 0, background, screen );
apply_surface( 0, 200, message, screen );

//Mise � jour de l'�cran
if( SDL_Flip( screen ) == -1 ) {
	return 1;
}

	//Si tout s'est bien charg�
	return true;
}

