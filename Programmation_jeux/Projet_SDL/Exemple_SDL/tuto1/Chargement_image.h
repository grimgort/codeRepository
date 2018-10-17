#ifndef CHARGEMENT_IMAGE_H_INCLUDED
#define CHARGEMENT_IMAGE_H_INCLUDED
#include <SDL/SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>


SDL_Surface *load_image( std::string filename ) ;
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
#endif // CHARGEMENT_IMAGE_H_INCLUDED
