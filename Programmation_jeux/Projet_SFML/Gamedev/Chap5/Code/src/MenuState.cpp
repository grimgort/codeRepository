#include "MenuState.h"
#include <utility.h>
#include <Foreach.hpp>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
, m_options()
, m_optionIndex(0)
{
    sf::Texture& texture = context.textures->get(Textures::TitleScreen);
    sf::Font& font = context.fonts->get(Fonts::Main);

    m_backgroundSprite.setTexture(texture);

    sf::Text playOption;
    playOption.setFont(font);
    playOption.setString("Play");
    centerOrigin(playOption);
    playOption.setPosition(context.window->getView().getSize() / 2.f);
    m_options.push_back(playOption);

    sf::Text exitOption;
    exitOption.setFont(font);
    exitOption.setString("Exit");
    exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f,30.f));
    m_options.push_back(exitOption);

    updateOptionText();

}

void MenuState::draw()
{
    sf::RenderWindow& window = *getContext().window;;

    window.setView(window.getDefaultView());
    window.draw(m_backgroundSprite);

    FOREACH(const sf::Text& text, m_options) window.draw(text);
}

bool MenuState::update(sf::Time)
{
    return true;
}


bool MenuState::handleEvent(const sf::Event& event)
{
    if (event.type != sf::Event::KeyPressed) return false;

    if (event.key.code == sf::Keyboard::Return)
    {
        if (m_optionIndex == Play)
        {
            requestStackPop();
            requestStackPush(States::Game);
        }
        else if (m_optionIndex == Exit)
        {
            requestStackPop();
        }

    }
    else if (event.key.code == sf::Keyboard::Up)
    {
        if (m_optionIndex > 0) m_optionIndex--;
        else m_optionIndex = m_options.size() - 1;
    }
    else if (event.key.code == sf::Keyboard::Down)
    {
        if (m_optionIndex < m_options.size() - 1) m_optionIndex++;
        else m_optionIndex = 0;

        updateOptionText();
    }

    return true;

}

void MenuState::updateOptionText()
{
    if (m_options.empty())
        return;

    FOREACH(sf::Text& text, m_options) text.setColor(sf::Color::White);

    m_options[m_optionIndex].setColor(sf::Color::Red);
}




