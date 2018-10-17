#ifndef LOADINGSTATE_H
#define LOADINGSTATE_H

#include <State.h>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <ParallelTask.h>



class LoadingState : public State
{
public:
    LoadingState (StateStack& stack, Context context);

    virtual void draw();
    virtual bool update (sf::Time dt);
    virtual bool handleEvent (const sf::Event& event);

    void setCompletion (float percent);

private:
    sf::Text m_loadingText;
    sf::RectangleShape m_progressBarBackground;
    sf::RectangleShape m_progressBar;

    ParallelTask m_loadingTask;
};

#endif // LOADINGSTATE_H
