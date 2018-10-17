#ifndef APPLICATION_H
#define APPLICATION_H

#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.h>
#include <KeyBinding.h>
#include <StateStack.h>
#include <MusicPlayer.h>
#include <SoundPlayer.h>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


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
    MusicPlayer m_music;
    SoundPlayer m_sounds;

    KeyBinding m_keyBinding1;
    KeyBinding m_keyBinding2;
    StateStack m_stateStack;

    sf::Text m_statisticsText;
    sf::Time m_statisticsUpdateTime;
    std::size_t	 m_statisticsNumFrames;
};

#endif // APPLICATION_H
