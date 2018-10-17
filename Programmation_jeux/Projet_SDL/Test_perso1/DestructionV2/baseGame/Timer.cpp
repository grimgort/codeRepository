
#include "Timer.h"

Timer::Timer() : m_startTicks(0),m_pausedTicks(0),m_paused(false),m_started(false)
{
}

void Timer::start()
{
    //On demarre le timer
    m_started = true;

    //On enlève la pause du timer
    m_paused = false;

    //On récupére le temps courant
    m_startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //On stoppe le timer
    m_started = false;

    //On enlève la pause
    m_paused = false;
}

void Timer::pause()
{
    //Si le timer est en marche et pas encore en pause
    if( ( m_started == true ) && ( m_paused == false ) )
    {
        //On met en pause le timer
        m_paused = true;

        //On calcul le pausedTicks
        m_pausedTicks = SDL_GetTicks() - m_startTicks;
    }
}

void Timer::unpause()
{
    //Si le timer est en pause
    if( m_paused == true )
    {
        //on enlève la pause du timer
        m_paused = false;

        //On remet à zero le startTicks
        m_startTicks = SDL_GetTicks() - m_pausedTicks;

        //Remise à zero du pausedTicks
        m_pausedTicks = 0;
    }
}

void Timer::standFrame(int framePerSecond)
{
        //Tant que le timer n'est pas assez haut
        while( getTicks() < 1000 / framePerSecond )
        {
            //On attend...
        }
}

int Timer::getTicks()
{
    //Si le timer est en marche
    if( m_started == true )
    {
        //Si le timer est en pause
        if( m_paused == true )
        {
            //On retourne le nombre de ticks quand le timer a été mis en pause
            return m_pausedTicks;
        }
        else
        {
            //On retourne le temps courant moins le temps quand il a démarré
            return SDL_GetTicks() - m_startTicks;
        }
    }

    //Si le timer n'est pas en marche
    return 0;
}

bool Timer::getStarted()
{
    return m_started;
}

bool Timer::getPaused()
{
    return m_paused;
}
