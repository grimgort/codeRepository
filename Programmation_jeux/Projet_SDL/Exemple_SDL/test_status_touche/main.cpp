/*******************************************************/
/** Programme de PERRUCHON Romain pour developpez.com **/
/**        Chapitre III-C : Status d'une touche       **/
/*******************************************************/

//Les fichiers d'entete
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <string>

//Les attributs de l'ecran (640 * 480)
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//Les surfaces
SDL_Surface *background = NULL;
SDL_Surface *up = NULL;
SDL_Surface *down = NULL;
SDL_Surface *left = NULL;
SDL_Surface *right = NULL;
SDL_Surface *screen = NULL;

//La structure d'evenement
SDL_Event event;

//Le font
TTF_Font *font;

//La couleur du font font
SDL_Color textColor = { 0, 0, 0 };

SDL_Surface *load_image( std::string filename )
{
    //L'image qui est chargée
    SDL_Surface* loadedImage = NULL;

    //L'image optimisée que nous utiliserons par la suite
    SDL_Surface* optimizedImage = NULL;

    //Chargement de l'image
    loadedImage = IMG_Load( filename.c_str() );

    //Si l'image est chargée correctement
    if( loadedImage != NULL )
    {
        //creation de l'image optimisée
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //liberation de l'ancienne image
        SDL_FreeSurface( loadedImage );

        //si l'image optimisée créé est bonne
        if( optimizedImage != NULL )
        {
            //Transparence
            SDL_SetColorKey( optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //on retourne l'image optimisé
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    //On Blitte la surface
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialisation de tous les sous-systemes de SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Mise en place de l'ecran
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //Si il y a une erreur lors de la mise en place de l'ecran
    if( screen == NULL )
    {
        return false;
    }

    //Initialisation de SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //on met en place la barre caption de la fenetre
    SDL_WM_SetCaption( "Press an Arrow Key", NULL );

    //si tout s'est bien passé
    return true;
}

bool load_files()
{
    //Chargement du fond
    background = load_image( "background.png" );

    //Ouverture du font
    font = TTF_OpenFont( "CaslonRoman.ttf", 72 );

    //Si il y a un probleme au chargement du fond
    if( background == NULL )
    {
        return false;
    }

    //si il y a une erreur à l'ouverture du font
    if( font == NULL )
    {
        return false;
    }

    //Si tout s'est bien passé
    return true;
}

void clean_up()
{
    //Liberation des surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface( up );
    SDL_FreeSurface( down );
    SDL_FreeSurface( left );
    SDL_FreeSurface( right );

    //Fermeture du Font
    TTF_CloseFont( font );

    //On quitte SDL_ttf
    TTF_Quit();

    //On quitte SDL
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    //Ce qui va nous permettre de quitter
    bool quit = false;

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

    //Mise en place du texte
    up = TTF_RenderText_Solid( font, "Haut", textColor );
    down = TTF_RenderText_Solid( font, "Bas", textColor );
    left = TTF_RenderText_Solid( font, "Gauche", textColor );
    right = TTF_RenderText_Solid( font, "Droite", textColor );

    //Tant que l'utilisateur n'a pas quitter
    while( quit == false )
    {
        //tant qu'il y a un evenement à manipuler
        while( SDL_PollEvent( &event ) )
        {
            //Si l'utilisateur ferme la fenetre avec le X
            if( event.type == SDL_QUIT )
            {
                //On quitte le programme
                quit = true;
            }
        }

        //Application de la surface background (fond)
        apply_surface( 0, 0, background, screen );

        //Obtenir les KeyStates
        Uint8 *keystates = SDL_GetKeyState( NULL );

        //Si Haut est pressé (la fleche)
        if( keystates[ SDLK_UP ] )
        {
            apply_surface( ( SCREEN_WIDTH - up->w ) / 2, ( SCREEN_HEIGHT / 2 - up->h ) / 2, up, screen );
        }

        //Si Bas est pressé (la fleche)
        if( keystates[ SDLK_DOWN ] )
        {
            apply_surface( ( SCREEN_WIDTH - down->w ) / 2, ( SCREEN_HEIGHT / 2 - down->h ) / 2 + ( SCREEN_HEIGHT / 2 ), down, screen );
        }

        //Si Gauche est pressé (la fleche)
        if( keystates[ SDLK_LEFT ] )
        {
            apply_surface( ( SCREEN_WIDTH / 2 - left->w ) / 2, ( SCREEN_HEIGHT - left->h ) / 2, left, screen );
        }

        //Si Droite est pressé (la fleche)
        if( keystates[ SDLK_RIGHT ] )
        {
            apply_surface( ( SCREEN_WIDTH / 2 - right->w ) / 2 + ( SCREEN_WIDTH / 2 ), ( SCREEN_HEIGHT - right->h ) / 2, right, screen );
        }

        //Mise à jours de l'ecran
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    }

    //Netoyage
    clean_up();

    return 0;
}
