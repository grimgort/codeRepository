#ifndef APPLICATION_H
#define APPLICATION_H

#include <World.h>
#include <Player.h>
#include <StateStack.h>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Application
{
public:
    Application();
    void run();

private:
    void processInput();
    void update(sf::Time dt);
    void render();
    void updateStatistics(sf::Time dt);
    void registerStates();

private:
    static const sf::Time timePerFrame;
    sf::RenderWindow m_window;
    TextureHolder m_textures;
	FontHolder	m_fonts;
    Player m_player;

    sf::Text m_statisticsText;
    std::size_t	 m_statisticsNumFrames;
    sf::Time m_statisticsUpdateTime;

    StateStack m_stateStack;
};

#endif // APPLICATION_H
