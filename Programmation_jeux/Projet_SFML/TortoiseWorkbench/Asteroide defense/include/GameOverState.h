#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include <State.h>
#include <Container.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameOverState : public State
{
public:
    GameOverState (StateStack& stack, Context context,
                   const std::string& text);

    virtual void draw();
    virtual bool update (sf::Time dt);
    virtual bool handleEvent (const sf::Event& event);


private:
    sf::Text m_gameOverText;
    sf::Time m_elapsedTime;
};

#endif // GAMEOVERSTATE_H
