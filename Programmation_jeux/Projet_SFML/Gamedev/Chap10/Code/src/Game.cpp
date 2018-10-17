#include "Game.h"
#include "StringHelpers.hpp"

#include <SFML/Window/Event.hpp>

const sf::Time Game::timePerFrame = sf::seconds(1.f/60.f);

/************************************************
CONSTRUCTEUR/DESTRUCTEUR
************************************************/
Game::Game() : m_window(sf::VideoMode(700,700),"Premier Jeux", sf::Style::Close)
    , m_world(m_window)
    , m_frameFont()
    , m_statisticsText()
    , m_statisticsUpdateTime()
    , m_statisticsNumFrames(0)
{

    if (!m_frameFont.loadFromFile("media/Sansation.ttf"))
    {

    }
    m_statisticsText.setFont(m_frameFont);
    m_statisticsText.setPosition(10.f,10.f);
    m_statisticsText.setCharacterSize(50);

}



/************************************************
METHODE
************************************************/
// Lance le jeux
void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero; // define time of each frame

// RQ : If game lag, you can take it smoother with called multi time. Look lesson on it(p24).
//      You can activate V-Sync for adapts time step with your monitory(p24)
    while (m_window.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > timePerFrame )
        {
            timeSinceLastUpdate -= timePerFrame ;
            processInput();
            update(timePerFrame);
        }
        updateStatistics(elapsedTime);
        render();
    }
}

/*
Applique les commande du clavier aux jeux
*/
void Game::processInput()
{
    CommandQueue& commands = m_world.getCommandQueue(); //Récupére la liste des commandes

    sf::Event event;
    while (m_window.pollEvent(event))
    {
        m_player.handleEvent(event, commands);

        if (event.type==sf::Event::Closed) // Close window
        {
            m_window.close();
        }
    }

    m_player.handleRealtimeInput(commands);
}

// Mise a jour du jeux en fonction des événements
void Game::update(sf::Time deltaTime)
{
    m_world.update(deltaTime);
}

// applique les objets
void Game::render()
{
    m_window.clear(); // Fill screen with black color (default)
    m_world.draw(); // apply player on screen
    m_window.setView(m_window.getDefaultView());
    m_window.draw(m_statisticsText);
    m_window.display(); // update screen
}

void Game::updateStatistics(sf::Time elapsedTime)
{
    m_statisticsUpdateTime += elapsedTime;
    m_statisticsNumFrames += 1;

    if (m_statisticsUpdateTime >= sf::seconds(1.0f))
    {
        m_statisticsText.setString(
            "Frames / Second = " + toString(m_statisticsNumFrames) + "\n" +
            "Time / Update = " + toString(m_statisticsUpdateTime.asMicroseconds() / m_statisticsNumFrames) + "us");

        m_statisticsUpdateTime -= sf::seconds(1.0f);
        m_statisticsNumFrames = 0;
    }
}
