/*******************************************************/
/** Programme de PERRUCHON Romain pour developpez.com **/
/**         Chapitre XVIII : Multithreading           **/
/*******************************************************/

//Les fichiers d'entête
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_thread.h"
#include <string>

//Les attributs de l'écran (640 * 480)
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//Les surfaces
SDL_Surface *image = NULL;
SDL_Surface *screen = NULL;

//La structure d'événements que nous allons utiliser
SDL_Event event;

//Le thread qui sera utiliser
SDL_Thread *thread = NULL;

//Ce qui va nous permettre de quitter
bool quit = false;

SDL_Surface *load_image( std::string filename )
{
    //L'image qui est chargée
    SDL_Surface* loadedImage = NULL;

    //L'image optimisée qu'on va utiliser
    SDL_Surface* optimizedImage = NULL;

    //Chargement de l'image
    loadedImage = IMG_Load( filename.c_str() );

    //Si l'image a bien chargée
    if( loadedImage != NULL )
    {
        //Création de l'image optimisée
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Libération de l'ancienne image
        SDL_FreeSurface( loadedImage );

        //Si la surface a bien été optimisée
        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //On retourne l'image optimisée
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //On stock les coordonnees
    SDL_Rect offset;

    //Recuperation des coordonnees
    offset.x = x;
    offset.y = y;

    //On blitte la surface
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialisation de tous les sous-système de SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
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
    SDL_WM_SetCaption( "Thread test", NULL );

    //Si tout s'est bien passé
    return true;
}

bool load_files()
{
    //Charge l'image de fond
    image = load_image( "image.png" );

    //S'il y a eu un problème au chargement de l'image
    if( image == NULL )
    {
        return false;
    }

    //Si tout s'est bien passé
    return true;
}

void clean_up()
{
    //On stop le thread
    SDL_KillThread( thread );

    //On libère la surface
    SDL_FreeSurface( image );

    //On Quitte SDL
    SDL_Quit();
}

int my_thread( void *data )
{
    //Tant que le programme n'est pas fini
    while( quit == false )
    {
        //On anime la barre caption
        SDL_WM_SetCaption( "Thread is running", NULL );
        SDL_Delay( 250 );

        SDL_WM_SetCaption( "Thread is running.", NULL );
        SDL_Delay( 250 );

        SDL_WM_SetCaption( "Thread is running..", NULL );
        SDL_Delay( 250 );

        SDL_WM_SetCaption( "Thread is running...", NULL );
        SDL_Delay( 250 );
    }

    return 0;
}

int main( int argc, char* args[] )
{
    //Initialisation
    if( init() == false )
    {
        return 1;
    }

    //Chargement des fichiers
    if( load_files() == false )
    {
        return 1;
    }

    //Creation et lancement du thread
    thread = SDL_CreateThread( my_thread, NULL );

    //On applique l'image sur l'écran
    apply_surface( 0, 0, image, screen );

    //Mise à jour de l'écran
    if( SDL_Flip( screen ) == -1 )
    {
        return 1;
    }

    //Tant que l'utilisateur n'a pas quitté
    while( quit == false )
    {
        //Tant qu'il y a un événement
        while( SDL_PollEvent( &event ) )
        {
            //Si l'utilisateur a cliqué sur le X de la fenêtre
            if( event.type == SDL_QUIT )
            {
                //On quitte the programme
                quit = true;
            }
        }
    }

    //Netoyage
    clean_up();

    return 0;
}
