/*******************************************************/
/** Programme de PERRUCHON Romain pour developpez.com **/
/**            Chapitre II : Hello World              **/
/*******************************************************/

//Les fichiers d'entete
#include "SDL/SDL.h"
#include <string>

//Les attributs de l'ecran (640 * 480)
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//Les surfaces
SDL_Surface *message = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;

//La structure d'evenement
SDL_Event event;

SDL_Surface *load_image( std::string filename ) {
    //L'image qui est chargée
    SDL_Surface* loadedImage = NULL;

    //L'image optimisée que nous utiliserons par la suite
    SDL_Surface* optimizedImage = NULL;

    //Chargement de l'image
    loadedImage = SDL_LoadBMP( filename.c_str() );

    //Si l'image est chargée correctement
    if( loadedImage != NULL ) {
        //creation de l'image optimisée
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //liberation de l'ancienne image
        SDL_FreeSurface( loadedImage );
    }
    //on retourne l'image optimisé
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination ) {

    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    //on blit la surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

int main( int argc, char* args[] ) {

    bool quit = false;

    //initialisation de tout les sous-systemes de sdl
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
        return 1;
    }

    //on met en place l'ecran
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //Si il y a une erreur lors de la mise en place de l'ecran
    if( screen == NULL ) {
        return 1;
    }

    //on met en place la barre caption de la fenetre
    SDL_WM_SetCaption( "Hello World", NULL );

    //Chargement des images
    message = load_image( "hello_world.bmp" );
    background = load_image( "background.bmp" );

    //Application des surfaces sur l'ecran
    apply_surface( 0, 0, background, screen );

    //Application du message sur l'ecran
    apply_surface( 180, 140, message, screen );

    //mise à jour de l'ecran
    if( SDL_Flip( screen ) == -1 ) {
        return 1;
    }

    //Tant que l'utilisateur n'a pas quitter
    while( quit == false ) {
        //tant qu'il y a un evenement dans le handler
        while( SDL_PollEvent( &event ) ) {
            //Si l'utilisateur ferme la fenetre avec le X
            if( event.type == SDL_QUIT ) {
                //On quitte le programme
                quit = true;
            }
        }
    }

    //Liberation des surface
    SDL_FreeSurface( message );
    SDL_FreeSurface( background );

    //On quitte sdl
    SDL_Quit();
}