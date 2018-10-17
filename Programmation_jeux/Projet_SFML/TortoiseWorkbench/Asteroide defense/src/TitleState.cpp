#include "TitleState.h"
#include <utility.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

TitleState::TitleState (StateStack& stack, Context context)
    : State (stack, context)
    , m_text()
    , m_showText (true)
    , m_textEffectTime (sf::Time::Zero)
{
    m_backgroundSprite.setTexture (context.textures->get (
                                       Textures::TitleScreen));

    m_text.setFont (context.fonts->get (Fonts::Main));
    m_text.setString ("Press any key to start");
    centerOrigin (m_text);
    m_text.setPosition (sf::Vector2f (context.window->getSize() / 2u));
}

void TitleState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw (m_backgroundSprite);

    if (m_showText)
    { window.draw (m_text); }
}

bool TitleState::update (sf::Time dt)
{
    m_textEffectTime += dt;

    if (m_textEffectTime > sf::seconds (0.5f))
    {
        m_showText = !m_showText;
        m_textEffectTime = sf::Time::Zero;
    }

    return true;
}


bool TitleState::handleEvent (const sf::Event& event)
{
    if (event.type  == sf::Event::KeyReleased)
    {
        requestStackPop();
        requestStackPush (States::Menu);
    }

    return true;
}







