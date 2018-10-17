/*******************************************************/
/** Programme de PERRUCHON Romain pour developpez.com **/
/**               Chapitre X : Mouvement              **/
/*******************************************************/

//Les fichiers d'entête
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

//Les attributs de l'écran (640 * 480)
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//Le nombre de frame par seconde
const int FRAMES_PER_SECOND = 20;

//Les dimensions du tux (l'image qu'on va deplacer)
const int TUX_WIDTH = 32;
const int TUX_HEIGHT = 32;

//Les surfaces
SDL_Surface *tux = NULL;
SDL_Surface *fond = NULL;
SDL_Surface *screen = NULL;

//La structure d'événements que nous allons utiliser
SDL_Event event;

//Le tux que nous allons bouger sur l'ecran
class Tux
{
    private:
    //Les coordonnées x et y du tux
    int x, y;

    //La vitesse du tux
    int xVel, yVel;

    public:
    //Initialisation des variables
    Tux();

    //Recupere la touche pressee et ajuste la vitesse du tux
    void handle_input();

    //Montre le tux sur l'ecran
    void show();
};

//Le timer
class Timer
{
    private:
    //Le temps quand le timer est lancé
    int startTicks;

    //Les "ticks" enregistré quand le Timer a été mit en pause
    int pausedTicks;

    //Le status du Timer
    bool paused;
    bool started;

    public:
    //Initialise les variables (le constructeur)
    Timer();

    //Les différentes actions du timer
    void start();
    void stop();
    void pause();
    void unpause();

    //recupére le nombre de ticks depuis que le timer a été lancé
    //ou récupére le nombre de ticks depuis que le timer a été mis en pause
    int get_ticks();

    //Fonctions de vérification du status du timer
    bool is_started();
    bool is_paused();
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
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0xFF, 0, 0xFF );

            //on met tout les pixel de couleur R 0, G 0xFF, B 0xFF transparent
            SDL_SetColorKey( optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
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
    SDL_WM_SetCaption( "Bouge le Tux", NULL );

    //Si tout s'est bien passé
    return true;
}

bool load_files()
{
    //Chargement du tux
    tux = load_image( "tux.png" );

    //Si vous souhaitez changer d'image, il suffit de changer l'image chargé :
    //tux = load_image( "sprite.bmp" );

    //S'il y a eu un problème au chargement du tux
    if( tux == NULL )
    {
        return false;
    }

    //Chargement du fond
    fond = load_image( "grass.bmp" );

    //S'il y a eu un problème au chargement du fond
    if( fond == NULL )
    {
        return false;
    }

    //Si tout s'est bien passé
    return true;
}

void clean_up()
{
    //Libèration des surfaces
    SDL_FreeSurface( fond );
    SDL_FreeSurface( tux );

    //On quitte SDL
    SDL_Quit();
}

Tux::Tux()
{
    //Initialisation des coordonnees
    x = 0;
    y = 0;

    //Initialisation de la vitesse
    xVel = 0;
    yVel = 0;
}

void Tux::handle_input()
{
    //Si une touche a ete pressee
    if( event.type == SDL_KEYDOWN )
    {
        //ajustement de la vitesse
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel -= TUX_HEIGHT / 2; break;
            case SDLK_DOWN: yVel += TUX_HEIGHT / 2; break;
            case SDLK_LEFT: xVel -= TUX_WIDTH / 2; break;
            case SDLK_RIGHT: xVel += TUX_WIDTH / 2; break;
            default: break;
        }
    }
    //Si une touche a ete relachee
    else if( event.type == SDL_KEYUP )
    {
        //ajustement de la vitesse
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel += TUX_HEIGHT / 2; break;
            case SDLK_DOWN: yVel -= TUX_HEIGHT / 2; break;
            case SDLK_LEFT: xVel += TUX_WIDTH / 2; break;
            case SDLK_RIGHT: xVel -= TUX_WIDTH / 2; break;
            default: break;
        }
    }
}

void Tux::show()
{
    //Bouge le tux à gauche ou à droite
    x += xVel;

    //Si le tux se rapproche trop des limites(gauche ou droite) de l'ecran
    if( ( x < 0 ) || ( x + TUX_WIDTH > SCREEN_WIDTH ) )
    {
        //On revient
        x -= xVel;
    }

    //Bouge le tux en haut ou en bas
    y += yVel;

    //Si le tux se rapproche trop des limites(haute ou basse) de l'ecran
    if( ( y < 0 ) || ( y + TUX_HEIGHT > SCREEN_HEIGHT ) )
    {
        //On revient
        y -= yVel;
    }

    apply_surface( 0, 0, fond, screen );
    //Affiche le tux
    apply_surface( x, y, tux, screen );
}

Timer::Timer()
{
    //Initialisation des variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //On demarre le timer
    started = true;

    //On enlève la pause du timer
    paused = false;

    //On récupére le temps courant
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //On stoppe le timer
    started = false;

    //On enlève la pause
    paused = false;
}

void Timer::pause()
{
    //Si le timer est en marche et pas encore en pause
    if( ( started == true ) && ( paused == false ) )
    {
        //On met en pause le timer
        paused = true;

        //On calcul le pausedTicks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //Si le timer est en pause
    if( paused == true )
    {
        //on enlève la pause du timer
        paused = false;

        //On remet à zero le startTicks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Remise à zero du pausedTicks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //Si le timer est en marche
    if( started == true )
    {
        //Si le timer est en pause
        if( paused == true )
        {
            //On retourne le nombre de ticks quand le timer a été mis en pause
            return pausedTicks;
        }
        else
        {
            //On retourne le temps courant moins le temps quand il a démarré
            return SDL_GetTicks() - startTicks;
        }
    }

    //Si le timer n'est pas en marche
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

int main( int argc, char* args[] )
{
    //Ce qui va nous permettre de quitter
    bool quit = false;

    //Le tux qu'on va utilise
    Tux monTux;

    //Le regulateur
    Timer fps;

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

    //Tant que l'utilisateur n'a pas quitter
    while( quit == false )
    {
        //On demarre le timer fps
        fps.start();

        //Tant qu'il y a un événement
        while( SDL_PollEvent( &event ) )
        {
            //On recupere l'evenement pour le tux
            monTux.handle_input();

            //Si l'utilisateur a cliqué sur le X de la fenêtre
            if( event.type == SDL_QUIT )
            {
                //On quitte the programme
                quit = true;
            }
        }

        apply_surface( 0, 0, fond, screen );

        //On affiche le tux sur l'ecran
        monTux.show();

        //Mise à jour de l'écran
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

        //Tant que le timer fps n'est pas assez haut
        while( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            //On attend...
        }
    }

    //Nettoyage
    clean_up();

    return 0;
}
