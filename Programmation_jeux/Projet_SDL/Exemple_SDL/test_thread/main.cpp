/*******************************************************/
/** Programme de PERRUCHON Romain pour developpez.com **/
/**         Chapitre XVIII : Multithreading           **/
/*******************************************************/

//Les fichiers d'ent�te
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_thread.h"
#include <string>

//Les attributs de l'�cran (640 * 480)
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//Les surfaces
SDL_Surface *image = NULL;
SDL_Surface *screen = NULL;

//La structure d'�v�nements que nous allons utiliser
SDL_Event event;

//Le thread qui sera utiliser
SDL_Thread *thread = NULL;

//Ce qui va nous permettre de quitter
bool quit = false;

SDL_Surface *load_image( std::string filename )
{
    //L'image qui est charg�e
    SDL_Surface* loadedImage = NULL;

    //L'image optimis�e qu'on va utiliser
    SDL_Surface* optimizedImage = NULL;

    //Chargement de l'image
    loadedImage = IMG_Load( filename.c_str() );

    //Si l'image a bien charg�e
    if( loadedImage != NULL )
    {
        //Cr�ation de l'image optimis�e
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Lib�ration de l'ancienne image
        SDL_FreeSurface( loadedImage );

        //Si la surface a bien �t� optimis�e
        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //On retourne l'image optimis�e
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
    //Initialisation de tous les sous-syst�me de SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Mise en place de l'�cran
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //S'il y a eu une erreur lors de la mise en place de l'�cran
    if( screen == NULL )
    {
        return false;
    }

    //Mise en place de la barre caption
    SDL_WM_SetCaption( "Thread test", NULL );

    //Si tout s'est bien pass�
    return true;
}

bool load_files()
{
    //Charge l'image de fond
    image = load_image( "image.png" );

    //S'il y a eu un probl�me au chargement de l'image
    if( image == NULL )
    {
        return false;
    }

    //Si tout s'est bien pass�
    return true;
}

void clean_up()
{
    //On stop le thread
    SDL_KillThread( thread );

    //On lib�re la surface
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

    //On applique l'image sur l'�cran
    apply_surface( 0, 0, image, screen );

    //Mise � jour de l'�cran
    if( SDL_Flip( screen ) == -1 )
    {
        return 1;
    }

    //Tant que l'utilisateur n'a pas quitt�
    while( quit == false )
    {
        //Tant qu'il y a un �v�nement
        while( SDL_PollEvent( &event ) )
        {
            //Si l'utilisateur a cliqu� sur le X de la fen�tre
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
