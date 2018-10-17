#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <SDL/SDL.h>

//Le timer
class Timer
{
public:
    //Initialise les variables (le constructeur)
    Timer();

    //Les diff�rentes actions du timer
    void start();
    void stop();
    void pause();
    void unpause();
    void standFrame(int framePerSecond);
    //recup�re le nombre de ticks depuis que le timer a �t� lanc�
    //ou r�cup�re le nombre de ticks depuis que le timer a �t� mis en pause
    int getTicks();
    //Fonctions de v�rification du status du timer
    bool getStarted();
    bool getPaused();


private:
    //Le temps quand le timer est lanc�
    int m_startTicks;
    //Les "ticks" enregistr� quand le Timer a �t� mit en pause
    int m_pausedTicks;
    //Le status du Timer
    bool m_paused;
    bool m_started;
};

#endif // TIMER_H_INCLUDED
