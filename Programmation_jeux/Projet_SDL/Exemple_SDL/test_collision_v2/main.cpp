/*******************************************************/
/** Programme de PERRUCHON Romain pour developpez.com **/
/**             Chapitre XI : Collisions             **/
/*******************************************************/

//Les fichiers d'ent�te
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

//Les attributs de l'�cran (640 * 480)
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//Le nombre de frame par seconde
const int FRAMES_PER_SECOND = 30;

//Les dimensions du carr� (l'image qu'on va deplacer)
const int SQUARE_WIDTH = 20;
const int SQUARE_HEIGHT = 20;

//Vitesse de deplacement du carr� (en pixel par seconde)
const int SQUARE_VITESSE = 200;

//Les surfaces
SDL_Surface *square = NULL;
SDL_Surface *screen = NULL;

//La structure d'�v�nements que nous allons utiliser
SDL_Event event;

//Le mur
SDL_Rect wall;

//La classe Square (carr�)
class Square
{
    private:
    //la boite de collision du carr�
    SDL_Rect box;

    //La vitesse du point
    int xVel, yVel;

    public:
    //Initialisation des variables
    Square();

    //Recupere la touche pressee et ajuste la vitesse du carr�
    void handle_input();

    //Montre le carr� sur l'ecran
    void show();
};

//La classe timer
class Timer
{
    private:
    //Le temps quand le timer est lanc�
    int startTicks;

    //Les "ticks" enregistr� quand le Timer a �t� mit en pause
    int pausedTicks;

    //Le status du Timer
    bool paused;
    bool started;

    public:
    //Initialise les variables (le constructeur)
    Timer();

    //Les diff�rentes actions du timer
    void start();
    void stop();
    void pause();
    void unpause();

    //recup�re le nombre de ticks depuis que le timer a �t� lanc�
    //ou r�cup�re le nombre de ticks depuis que le timer a �t� mis en pause
    int get_ticks();

    //Fonctions de v�rification du status du timer
    bool is_started();
    bool is_paused();
};

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
    //Holds offsets
    SDL_Rect offset;

    //On recupere les corrdonn�es
    offset.x = x;
    offset.y = y;

    //On blitte la surface
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool check_collision( SDL_Rect &A, SDL_Rect &B )
{
    //Les cotes des rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calcul les cotes du rectangle A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    //Calcul les cotes du rectangle B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    //Tests de collision
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //Si conditions collisions detectee
    return true;
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
    SDL_WM_SetCaption( "Bouge le point", NULL );

    //Si tout s'est bien pass�
    return true;
}

bool load_files()
{
    //Chargement du carr�
    square = load_image( "square.bmp" );

    //S'il y a eu un probl�me au chargement du carr�
    if( square == NULL )
    {
        return false;
    }

    //Si tout s'est bien pass�
    return true;
}

void clean_up()
{
    //Lib�ration des surfaces
    SDL_FreeSurface( square );

    //On quitte SDL
    SDL_Quit();
}

Square::Square()
{
    //Initialisation des coordonnees
    box.x = 0;
    box.y = 0;

    //Les dimensions du carre
    box.w = SQUARE_WIDTH;
    box.h = SQUARE_HEIGHT;

    //Initialisation de la vitesse
    xVel = 0;
    yVel = 0;
}

void Square::handle_input()
{
    //Si une touche a ete pressee
    if( event.type == SDL_KEYDOWN )
    {
        //ajustement de la vitesse
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel -= (SQUARE_VITESSE / FRAMES_PER_SECOND); break;
            case SDLK_DOWN: yVel += (SQUARE_VITESSE / FRAMES_PER_SECOND); break;
            case SDLK_LEFT: xVel -= (SQUARE_VITESSE / FRAMES_PER_SECOND); break;
            case SDLK_RIGHT: xVel += (SQUARE_VITESSE / FRAMES_PER_SECOND); break;
        }
    }
    //Si une touche a ete relachee
    else if( event.type == SDL_KEYUP )
    {
        //ajustement de la vitesse
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel += (SQUARE_VITESSE / FRAMES_PER_SECOND); break;
            case SDLK_DOWN: yVel -= (SQUARE_VITESSE / FRAMES_PER_SECOND); break;
            case SDLK_LEFT: xVel += (SQUARE_VITESSE / FRAMES_PER_SECOND); break;
            case SDLK_RIGHT: xVel -= (SQUARE_VITESSE / FRAMES_PER_SECOND); break;
        }
    }
}

void Square::show()
{
    //Bouge le carre a droite ou a gauche
    box.x += xVel;

    int leftW, leftB;
    int rightW, rightB;
    int topW, topB;
    int bottomW, bottomB;

    leftB = box.x;
    rightB = box.x + box.w;
    topB = box.y;
    bottomB = box.y + box.h;

    leftW = wall.x;
    rightW = wall.x + wall.w;
    topW = wall.y;
    bottomW = wall.y + wall.h;

    //Si collision avec les cot�s de l'ecran (droite ou gauche)
    if( ( box.x < 0 ) ) {
        //On colle le carre contre l'ecran
        box.x = 0;
    }
    if ( box.x + SQUARE_WIDTH > SCREEN_WIDTH ) {
        //On colle le carre contre l'ecran
        box.x = SCREEN_WIDTH - SQUARE_WIDTH;
    }
    //Si collision avec le mur
    if( ( check_collision( box, wall )) ) {
        // collision droite
        if(box.x + box.w > wall.x + wall.w) {
            box.x = wall.x + wall.w;
        }
        // collision gauche
        if(box.x < wall.x) {
            box.x = wall.x - box.w;
        }
    }

    //Bouge le carre vers le haut ou vers le bas
    box.y += yVel;

    //Si collision avec les cot�s de l'ecran (haut ou bas)
    if( ( box.y < 0 ) ) {
        //On colle le carre contre l'ecran
        box.y = 0;
    }
    if ( box.y + SQUARE_HEIGHT > SCREEN_HEIGHT ) {
        //On colle le carre contre l'ecran
        box.y = SCREEN_HEIGHT - SQUARE_HEIGHT;
    }
    //Si collision avec le mur
    if( ( check_collision( box, wall )) ) {
        // collision haut
        if(box.y < wall.y){
            box.y = wall.y - box.h;
        }

        // collision bas
        if(box.y + box.h > wall.y + wall.h) {
            box.y = wall.y + wall.h;
        }
    }

    //Affiche le carre
    apply_surface( box.x, box.y, square, screen );
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

    //On enl�ve la pause du timer
    paused = false;

    //On r�cup�re le temps courant
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //On stoppe le timer
    started = false;

    //On enl�ve la pause
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
        //on enl�ve la pause du timer
        paused = false;

        //On remet � zero le startTicks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Remise � zero du pausedTicks
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
            //On retourne le nombre de ticks quand le timer a �t� mis en pause
            return pausedTicks;
        }
        else
        {
            //On retourne le temps courant moins le temps quand il a d�marr�
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

    //Le carre
    Square mySquare;

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

    //Dimensions et coordonn�es du mur
    wall.x = 300;
    wall.y = 40;
    wall.w = 40;
    wall.h = 400;

    //Tant que l'utilisateur n'a pas quitter
    while( quit == false )
    {
        //On demarre le timer fps
        fps.start();

        //Tant qu'il y a un �v�nement
        while( SDL_PollEvent( &event ) )
        {
            //On recupere l'evenement pour le point
            mySquare.handle_input();

            //Si l'utilisateur a cliqu� sur le X de la fen�tre
            if( event.type == SDL_QUIT )
            {
                //On quitte the programme
                quit = true;
            }
        }

        //On remplit l'ecran de blanc
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

        //Affiche le mur
        SDL_FillRect( screen, &wall, SDL_MapRGB( screen->format, 0x77, 0x77, 0x77 ) );

        //Affiche le carre
        mySquare.show();

        //Mise � jour de l'�cran
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
