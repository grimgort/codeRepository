#ifndef GAME_H
#define GAME_H

#include <World.h>
#include <Player.h>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>



class Game : private sf::NonCopyable
{
public:
    Game();
    void run();

private:
    void processInput();
    void update(sf::Time deltaTime);
    void render();
    void updateStatistics(sf::Time elapsedTime);

private:
    static const sf::Time timePerFrame;
    sf::RenderWindow m_window;
    World m_world;
    Player m_player;

    sf::Font m_frameFont;
    sf::Text m_statisticsText;
    std::size_t	 m_statisticsNumFrames;
    sf::Time m_statisticsUpdateTime;


};

#endif // GAME_H
