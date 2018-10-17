#include "PresentationState.h"
#include "utility"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


PresentationState::PresentationState(StateStack& stack, Context context)
    : State (stack, context)
{
    m_backgroundSprite.setTexture (context.textures->get (
                                       Textures::PresentationScreen));

}

void PresentationState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw (m_backgroundSprite);
}

bool PresentationState::update (sf::Time)
{
    return true;
}

bool PresentationState::handleEvent (const sf::Event& event)
{
    //Lorsque le compteur est à 2, on passe à l'état Titre
    float Timer=2.;
    if (m_spriteClock.getElapsedTime().asSeconds()>Timer)
    {
        requestStackPop();
        requestStackPush (States::Title);
    }

    return true;
}
