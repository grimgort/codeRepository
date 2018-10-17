//Les fichiers d'entete
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <string>

#include "fonction_basic.h"

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{

    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    //on blit la surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

SDL_Surface *load_image( std::string filename )
{
    //L'image qui est chargée
    SDL_Surface* loadedImage = NULL;

    //L'image optimisée que nous utiliserons par la suite
    SDL_Surface* optimizedImage = NULL;

    //Chargement de l'image
    loadedImage = IMG_Load( filename.c_str() );

    //Si l'image est chargée
    if( loadedImage != NULL )
    {
        //creation de l'image optimisée
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //liberation de l'ancienne image
        SDL_FreeSurface( loadedImage );

        //si l'image optimisée créé est bonne
        if( optimizedImage != NULL )
        {
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 255, 0, 255 );
//           Uint32 colorkey2 = SDL_MapRGB( optimizedImage->format, 0, 0XFF, 0XFF );
            //on met tout les pixel de couleur R 0, G 0xFF, B 0xFF transparent
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
//            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey2 );
        }
    }

    //on retourne l'image optimisé
    return optimizedImage;
}

SDL_Surface *load_image2( std::string filename )
{
    //L'image qui est chargée
    SDL_Surface* loadedImage = NULL;

    //L'image optimisée que nous utiliserons par la suite
    SDL_Surface* optimizedImage = NULL;

    //Chargement de l'image
    loadedImage = IMG_Load( filename.c_str() );

    //Si l'image est chargée
    if( loadedImage != NULL )
    {
        //creation de l'image optimisée
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //liberation de l'ancienne image
        SDL_FreeSurface( loadedImage );

        //si l'image optimisée créé est bonne
        if( optimizedImage != NULL )
        {
//            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 255, 0, 255 );
           Uint32 colorkey2 = SDL_MapRGB( optimizedImage->format, 0, 0XFF, 0XFF );
            //on met tout les pixel de couleur R 0, G 0xFF, B 0xFF transparent
//            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey2 );
        }
    }

    //on retourne l'image optimisé
    return optimizedImage;
}

void clean_up(SDL_Surface* fond, SDL_Surface* message,TTF_Font *font)
{
    //Libere le font
    SDL_FreeSurface( fond );
    SDL_FreeSurface( message );

    //Fermeture des Fonts qu'on a utilisé
    TTF_CloseFont( font );

    //On quitte SDL_ttf
    TTF_Quit();

    //On Quitte SDL
    SDL_Quit();
}

int updateScreen(SDL_Surface* screen)
{
    //mise à jour de l'ecran
    if( SDL_Flip( screen ) == -1 )
    {
        return 1;
    }
}

int showText(const char* texte,SDL_Surface* screen,SDL_Surface* message,TTF_Font *font)
{

    //La couleur du Font
    SDL_Color textColor = { 255, 255, 255 };
//Mise en place du texte sur la surface message
    message = TTF_RenderText_Solid( font, texte, textColor );
//S'il y a une erreur dans la mise en place du texte
    if( message == NULL )
    {
        return 1;
    }
    apply_surface( 10, 10, message, screen );
}


