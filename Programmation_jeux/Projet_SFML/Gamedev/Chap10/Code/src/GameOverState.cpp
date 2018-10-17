#include "GameOverState.h"
#include <Utility.h>
#include <Player.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

GameOverState::GameOverState(StateStack& stack, Context context, const std::string& text):
    State(stack, context),
    m_gameOverText(),
    m_elapsedTime(sf::Time::Zero)
{
    sf::Font& font = context.fonts->get(Fonts::Main);
    sf::Vector2f windowSize(context.window->getSize());

    m_gameOverText.setFont(font);
    m_gameOverText.setString(text);
    m_gameOverText.setCharacterSize(70);
    centerOrigin(m_gameOverText);
    m_gameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void GameOverState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());

    window.draw(backgroundShape);
    window.draw(m_gameOverText);
}

bool GameOverState::update(sf::Time dt)
{
    m_elapsedTime += dt;
    if (m_elapsedTime > sf::seconds(3))
    {
        requestStateClear();
        requestStackPush(States::Menu);
    }
    return false;
}

bool GameOverState::handleEvent(const sf::Event&)
{
    return false;
}

