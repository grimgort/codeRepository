/*******************************************************/
/** Programme de PERRUCHON Romain pour developpez.com **/
/**       Chapitre III-B : Pression d'une touche      **/
/*******************************************************/

//Les fichiers d'entête
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
SDL_Surface *message = NULL;
SDL_Surface *screen = NULL;

//La structure d'evenement
SDL_Event event;

//Le font
TTF_Font *font;

//La couleur du Font
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

    //On Blit la surface
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialisation de tout les sous-systemes
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
    SDL_WM_SetCaption( "Presser une touche flechée", NULL );

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
    SDL_FreeSurface( message );
    SDL_FreeSurface( up );
    SDL_FreeSurface( down );
    SDL_FreeSurface( left );
    SDL_FreeSurface( right );

    //Fermeture du font
    TTF_CloseFont( font );

    //On quitte SDL_ttf
    TTF_Quit();

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

    //Chargement des fichiers
    if( load_files() == false )
    {
        return 1;
    }

    //Generation des surfaces message
    up = TTF_RenderText_Solid( font, "Haut a été pressé.", textColor );
    down = TTF_RenderText_Solid( font, "Bas a été pressé.", textColor );
    left = TTF_RenderText_Solid( font, "Gauche a été pressé.", textColor );
    right = TTF_RenderText_Solid( font, "Droite a été pressé.", textColor );

    //Application du fond sur l'ecran
    apply_surface( 0, 0, background, screen );

    //Tant que l'utilisateur n'a pas quitter
    while( quit == false )
    {
        //tant qu'il y a un evenement à manipuler
        if( SDL_PollEvent( &event ) )
        {
            //Si une touche est pressée
            if( event.type == SDL_KEYDOWN )
            {
                //Mise en place du bon message (par rapport à la touche pressée)
                switch( event.key.keysym.sym )
                {
                    case SDLK_UP: message = up; break;
                    case SDLK_DOWN: message = down; break;
                    case SDLK_LEFT: message = left; break;
                    case SDLK_RIGHT: message = right; break;
                }
            }

            //Si l'utilisateur ferme la fenetre avec le X
            else if( event.type == SDL_QUIT )
            {
                //On quitte le programme
                quit = true;
            }
        }

        //Si un message a besoin d'être affiché
        if( message != NULL )
        {
            //Application du fond sur l'ecran
            apply_surface( 0, 0, background, screen );

            //Application du message centré sur l'ecran
            apply_surface( ( SCREEN_WIDTH - message->w ) / 2, ( SCREEN_HEIGHT - message->h ) / 2, message, screen );

            //On met à NULL le pointeur sur la surface
            message = NULL;
        }

        //Mise à jour de l'ecran
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    }

    //Nettoyage
    clean_up();

    return 0;
}
