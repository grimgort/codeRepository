#include <MenuState.h>
#include <Button.h>
#include <Utility.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack& stack, Context context)
    : State(stack, context)
    , m_GUIContainer()
{
    sf::Texture& texture = context.textures->get(Textures::TitleScreen);
    m_backgroundSprite.setTexture(texture);

    auto playButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    playButton->setPosition(100, 250);
    playButton->setText("Play");
    playButton->setCallBack([this] ()
    {
        requestStackPop();
        requestStackPush(States::Game);
    });

    auto settingButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    settingButton->setPosition(100, 300);
    settingButton->setText("Settings");
    settingButton->setCallBack([this] ()
    {
        requestStackPush(States::Settings);
    });

    auto exitButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    exitButton->setPosition(100, 350);
    exitButton->setText("Exit");
    exitButton->setCallBack([this] ()
    {
        requestStackPop();
    });

    m_GUIContainer.pack(playButton);
    m_GUIContainer.pack(settingButton);
    m_GUIContainer.pack(exitButton);
}

void MenuState::draw()
{
    sf::RenderWindow& window = *getContext().window;;

    window.setView(window.getDefaultView());

    window.draw(m_backgroundSprite);
    window.draw(m_GUIContainer);

}

bool MenuState::update(sf::Time)
{
    return true;
}


bool MenuState::handleEvent(const sf::Event& event)
{
    m_GUIContainer.handleEvent(event);
    return false;
}





