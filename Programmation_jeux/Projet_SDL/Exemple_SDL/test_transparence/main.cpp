/*******************************************************/
/** Programme de PERRUCHON Romain pour developpez.com **/
/**     Chapitre IV : Transparence - SDL_Color_Key    **/
/*******************************************************/

//Les fichiers d'entête
//SDL_image pour utiliser IMG_load (chargement image autre que Bitmap : ici *.png
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

//Les attributs de l'ecran (640 * 480)
const int SCREEN_WIDTH = 1285;
const int SCREEN_HEIGHT = 697;
const int SCREEN_BPP = 32;

//Les surfaces
SDL_Surface *background = NULL;
SDL_Surface *tux = NULL;
SDL_Surface *screen = NULL;

//La structure d'evenement
SDL_Event event;

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

            //on met tout les pixel de couleur R 0, G 0xFF, B 0xFF transparent
            SDL_SetColorKey( optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
        }
    }

    //on retourne l'image optimisé
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //rectangle temporaire
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    //on blit la surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

bool init()
{
    //initialisation de tout les sous-systemes de sdl
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 1;
    }

    //on met en place l'ecran
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //Si il y a une erreur lors de la mise en place de l'ecran
    if( screen == NULL )
    {
        return 1;
    }

    //on met en place la barre caption de la fenetre
    SDL_WM_SetCaption( "\"Hello!\"", NULL );

    //si tout s'est bien passé
    return true;
}

bool load_files()
{
    //chargement du fond
    background = load_image( "more-trees.png" );

    //si le fond ne se charge pas
    if( background == NULL )
    {
        return false;
    }

    //on charge l'image qu'on va appliquer sur le fond
    tux = load_image( "tux-sit.png" );

    //si l'image se charge mal
    if( tux == NULL )
    {
        return false;
    }

    return true;
}

void clean_up()
{
    //Liberation des surfaces surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface( tux );

    //On quitte SDL
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    //ce qui va nous permettre de quitter
    bool quit = false;

    //Initialisation
    if( init() == false )
    {
        return 1;
    }

    //chargement des fichiers
    if( load_files() == false )
    {
        return 1;
    }

    //Application des surfaces sur l'ecran
    apply_surface( 0, 0, background, screen );
    apply_surface( 270, 190, tux, screen );

    //mise à jour de l'ecran
    if( SDL_Flip( screen ) == -1 )
    {
        return 1;
    }

    //Tant que l'utilisateur n'a pas quitter
    while( quit == false )
    {
        //tant qu'il y a un evenement
        while( SDL_PollEvent( &event ) )
        {
            //Si l'utilisateur ferme la fenetre avec le X
            if( event.type == SDL_QUIT )
            {
                //On quitte le programme
                quit = true;
            }
        }
    }

    //liberation des surface et on quitte sdl
    clean_up();

    return 0;
}
