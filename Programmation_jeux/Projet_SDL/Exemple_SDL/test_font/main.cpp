/*******************************************************/
/** Programme de PERRUCHON Romain pour developpez.com **/
/**            Chapitre XVII : Font Bitmap            **/
/*******************************************************/

//Les fichiers d'entête
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

//Les attributs de l'écran (640 * 480)
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//Les surfaces
SDL_Surface *bitmapFont = NULL;
SDL_Surface *screen = NULL;

//La structure d'événements que nous allons utiliser
SDL_Event event;

//Notre classe BitmapFont
class BitmapFont
{
    private:
    //La surface du font
    SDL_Surface *bitmap;

    //Les caractères individuels dans le font
    SDL_Rect chars[ 256 ];

    public:
    //Le constructeur par defaut
    BitmapFont();

    //Le constructeur avec un argument, genere le font quand l'objet est construit
    BitmapFont( SDL_Surface *surface );

    //Genere le font
    void build_font( SDL_Surface *surface );

    //Affiche le texte
    void show_text( int x, int y, std::string text, SDL_Surface *surface );
};

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
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    //On blitte la surface
    SDL_BlitSurface( source, clip, destination, &offset );
}

Uint32 get_pixel32( int x, int y, SDL_Surface *surface )
{
    //Convertie les pixels en 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Recupere le pixel demande
    return pixels[ ( y * surface->w ) + x ];
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
    SDL_WM_SetCaption( "Test font bitmap", NULL );

    //Si tout s'est bien passé
    return true;
}

bool load_files()
{
    //Charge le font
    bitmapFont = load_image( "loka_font.png" );

    //S'il y a eu un problème au chargement du font
    if( bitmapFont == NULL )
    {
        return false;
    }

    //Si tout s'est bien passé
    return true;
}

void clean_up()
{
    //Libere le font
    SDL_FreeSurface( bitmapFont );

    //On Quitte SDL
    SDL_Quit();
}

BitmapFont::BitmapFont()
{
    //Met bitmap a NULL
    bitmap = NULL;
}

BitmapFont::BitmapFont( SDL_Surface *surface )
{
    //Construit le font
    build_font( surface );
}

void BitmapFont::build_font( SDL_Surface *surface )
{
    //Si la surface est NULL
    if( surface == NULL )
    {
        return;
    }

    //Recupere le bitmap
    bitmap = surface;

    //Mise en place de la couleur du background (fond)
    Uint32 bgColor = SDL_MapRGB( bitmap->format, 0, 0xFF, 0xFF );

    //Dimension des cellules
    int cellW = bitmap->w / 16;
    int cellH = bitmap->h / 16;

    //Le caractere courant
    int currentChar = 0;

    //On parcours les lignes des cellules
    for( int rows = 0; rows < 16; rows++ )
    {
        //On parcours les colonnes des cellules
        for( int cols = 0; cols < 16; cols++ )
        {
            //caractere courant
            chars[ currentChar ].x = cellW * cols;
            chars[ currentChar ].y = cellH * rows;

            //Mise en place des dimensions du caractere
            chars[ currentChar ].w = cellW;
            chars[ currentChar ].h = cellH;

            //On parcours les colonnes des pixels
            for( int pCol = 0; pCol < cellW; pCol++ )
            {
                //On parcours les lignes des pixels
                for( int pRow = 0; pRow < cellH; pRow++ )
                {
                    //Recupere les coordonnees du pixel
                    int pX = ( cellW * cols ) + pCol;
                    int pY = ( cellH * rows ) + pRow;

                    //Si un pixel non "colorkey" est trouve
                    if( get_pixel32( pX, pY, bitmap ) != bgColor )
                    {
                        //Coordonnee x
                        chars[ currentChar ].x = pX;

                        //On arrete la boucle
                        pCol = cellW;
                        pRow = cellH;
                    }
                }
            }

            //On parcours les colonnes des pixels
            for( int pCol_w = cellW - 1; pCol_w >= 0; pCol_w-- )
            {
                //On parcours les lignes des pixels
                for( int pRow_w = 0; pRow_w < cellH; pRow_w++ )
                {
                    //Recupere les coordonnees du pixel
                    int pX = ( cellW * cols ) + pCol_w;
                    int pY = ( cellH * rows ) + pRow_w;

                    //Si un pixel non "colorkey" est trouve
                    if( get_pixel32( pX, pY, bitmap ) != bgColor )
                    {
                        //longueur du caractere courant
                        chars[ currentChar ].w = ( pX - chars[ currentChar ].x ) + 1;

                        //On arrete la boucle
                        pCol_w = -1;
                        pRow_w = cellH;
                    }
                }
            }

            //caractere suivant
            currentChar++;
        }
    }
}

void BitmapFont::show_text( int x, int y, std::string text, SDL_Surface *surface )
{
    //coordonnees temporaires
    int X = x, Y = y;

    //verification que le font a bien ete construit
    if( bitmap != NULL )
    {
        //On parcours le texte
        for( int show = 0; text[ show ] != '\0'; show++ )
        {
            //Si le caractere courant est un espace
            if( text[ show ] == ' ' )
            {
                //On bouge de la taille d'un caractere
                X += bitmap->w / 32;
            }
            //Si le caractere courant est un "newline"
            else if( text[ show ] == '\n' )
            {
                //On descent
                Y += bitmap->h / 16;

                //On revient en arriere
                X = x;
            }
            else
            {
                //recupere la valeur ASCII du caractere
                int ascii = (int)text[ show ];

                //Affiche le caractere
                apply_surface( X, Y, bitmap, surface, &chars[ ascii ] );

                //On bouge de la longueur du caractere + un pas de un pixel
                X += chars[ ascii ].w + 1;
            }
        }
    }
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

    //Creation de notre font
    BitmapFont font( bitmapFont );

    //On remplit l'ecran de blanc
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

    //Affiche le texte
    //font.show_text( 50, 50, "Bitmap Font:\nABDCEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n0123456789", screen );
    font.show_text( 50, 50, "Bienvenue sur Developpez.com\n\nLoka", screen );

    //Mise à jour de l'écran
    if( SDL_Flip( screen ) == -1 )
    {
        return 1;
    }

    //Tant que l'utilisateur n'a pas quitte
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

    //Nettoyage
    clean_up();

    return 0;
}
