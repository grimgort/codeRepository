#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <SDL/SDL.h>

//Le timer
class Timer
{
public:
    //Initialise les variables (le constructeur)
    Timer();

    //Les différentes actions du timer
    void start();
    void stop();
    void pause();
    void unpause();
    void standFrame(int framePerSecond);
    //recupére le nombre de ticks depuis que le timer a été lancé
    //ou récupére le nombre de ticks depuis que le timer a été mis en pause
    int getTicks();
    //Fonctions de vérification du status du timer
    bool getStarted();
    bool getPaused();


private:
    //Le temps quand le timer est lancé
    int m_startTicks;
    //Les "ticks" enregistré quand le Timer a été mit en pause
    int m_pausedTicks;
    //Le status du Timer
    bool m_paused;
    bool m_started;
};

#endif // TIMER_H_INCLUDED
