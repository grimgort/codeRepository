#include <SDL/SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>

SDL_Surface *load_image( std::string filename ) {
	//Surface tampon qui nous servira pour charger l'image
	SDL_Surface* loadedImage = NULL;

	//L'image optimisée qu'on va utiliser
	SDL_Surface* optimizedImage = NULL;

	//Chargement de l'image
	loadedImage = SDL_LoadBMP( filename.c_str() );

	//Si le chargement se passe bien
	if( loadedImage != NULL ) {
		//Création de l'image optimisée
		optimizedImage = SDL_DisplayFormat( loadedImage );

		//Libération de l'ancienne image
		SDL_FreeSurface( loadedImage );
	}

	//On retourne l'image optimisée
	return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination ) {
	SDL_Rect offset;    //SDL_Rect créer un rectangle

	offset.x = x;
	offset.y = y;

	//On blitte la surface
	SDL_BlitSurface( source, NULL, destination, &offset );
/*
    source = image
    NULL = ?
    destination = Surface ou l'on applique l'image
    &offset = rectangle qui encadre l'image
*/
}

