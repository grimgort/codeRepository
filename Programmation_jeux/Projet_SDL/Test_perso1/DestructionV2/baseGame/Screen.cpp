
#include "Screen.h"
/*************************************
CONSTRUCTEUR
**************************************/
Screen::Screen() : m_screen(NULL), m_screen_WIDTH(1285), m_screen_HEIGHT(697),
    m_screen_BPP(32),m_FRAMES_PER_SECOND(20),m_caption("DestructionV2")
{
    init();
}

/*************************************
DESTRUCTEUR
**************************************/
Screen::~Screen()
{
    delete m_caption, m_screen;
    TTF_Quit(); //On quitte SDL_ttf
    SDL_Quit(); //On Quitte SDL
}

/*************************************
METHODE
**************************************/

/*
Description : permet de construire la fenêtre principal
*/
bool Screen::init()
{
    //Initialisation de tous les sous-système de SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Initialisation de SDL_TTF
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Mise en place de l'écran
    m_screen = SDL_SetVideoMode( m_screen_WIDTH, m_screen_HEIGHT, m_screen_BPP, SDL_SWSURFACE );

    //S'il y a eu une erreur lors de la mise en place de l'écran
    if( m_screen == NULL )
    {
        return false;
    }

    //Mise en place de la barre caption
    SDL_WM_SetCaption( m_caption, NULL );

    //Si tout s'est bien passé
    return true;
}
/*
Description : permet de mettre à jour la fenêtre principal et donc de prendre en compte les événements.
                Il faut donc le mettre aprés la boucle du SDL_PollEvent.
*/
void Screen::updateScreen()
{
    //mise à jour de l'ecran
    SDL_Flip( m_screen );
}
/*************************************
ACCESSEUR
**************************************/
SDL_Surface* Screen::getScreen() const
{
    return m_screen;
}

int Screen::getWidth() const
{
    return m_screen_WIDTH;
}

int Screen::getHeight() const
{
    return m_screen_HEIGHT;
}

void Screen::mousePosition()
{
   SDL_GetMouseState(&m_mouseX, &m_mouseY) ;
}

int Screen::getMouseX() const
{
    return m_mouseX;
}

int Screen::getMouseY() const
{
    return m_mouseY;
}
