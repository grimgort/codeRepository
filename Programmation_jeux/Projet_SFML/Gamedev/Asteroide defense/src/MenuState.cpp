#include <MenuState.h>
#include <Button.h>
#include <Utility.h>
#include <MusicPlayer.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack& stack, Context context)
    : State(stack, context)
    , m_GUIContainer()
{
    sf::Texture& texture = context.textures->get(Textures::TitleScreen);
    m_backgroundSprite.setTexture(texture);

    auto playButton = std::make_shared<GUI::Button>(context);
    playButton->setPosition(100, 250);
    playButton->setText("Play");
    playButton->setCallBack([this] ()
    {
        requestStackPop();
        requestStackPush(States::Game);
    });

    auto hostPlayButton = std::make_shared<GUI::Button>(context);
    hostPlayButton->setPosition(100, 350);
    hostPlayButton->setText("Host");
    hostPlayButton->setCallBack([this] ()
    {
        requestStackPop();
        requestStackPush(States::HostGame);
    });

    auto joinPlayButton = std::make_shared<GUI::Button>(context);
    joinPlayButton->setPosition(100, 400);
    joinPlayButton->setText("Join");
    joinPlayButton->setCallBack([this] ()
    {
        requestStackPop();
        requestStackPush(States::JoinGame);
    });

    auto settingsButton = std::make_shared<GUI::Button>(context);
    settingsButton->setPosition(100, 450);
    settingsButton->setText("Settings");
    settingsButton->setCallBack([this] ()
    {
        requestStackPush(States::Settings);
    });

    auto exitButton = std::make_shared<GUI::Button>(context);
    exitButton->setPosition(100, 500);
    exitButton->setText("Exit");
    exitButton->setCallBack([this] ()
    {
        requestStackPop();
    });

    m_GUIContainer.pack(playButton);
    m_GUIContainer.pack(hostPlayButton);
    m_GUIContainer.pack(joinPlayButton);
    m_GUIContainer.pack(settingsButton);
    m_GUIContainer.pack(exitButton);

    context.music->play(Music::MenuTheme);
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





