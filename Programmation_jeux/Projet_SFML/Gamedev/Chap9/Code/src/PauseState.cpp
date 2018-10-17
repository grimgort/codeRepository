#include <PauseState.h>
#include <Button.h>
#include <Utility.h>
#include <MusicPlayer.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context)
, m_backgroundSprite()
, m_pausedText()
, m_GUIContainer()
{
    sf::Font& font = context.fonts->get(Fonts::Main);
    sf::Vector2f windowSize(context.window->getSize());

    m_pausedText.setFont(font);
    m_pausedText.setString("Game Paused");
    m_pausedText.setCharacterSize(70);
    centerOrigin(m_pausedText);
    m_pausedText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

	auto returnButton = std::make_shared<GUI::Button>(context);
	returnButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
	returnButton->setText("Return");
	returnButton->setCallBack([this] ()
	{
		requestStackPop();
	});

	auto backToMenuButton = std::make_shared<GUI::Button>(context);
	backToMenuButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
	backToMenuButton->setText("Back to menu");
	backToMenuButton->setCallBack([this] ()
	{
		requestStateClear();
		requestStackPush(States::Menu);
	});

	m_GUIContainer.pack(returnButton);
	m_GUIContainer.pack(backToMenuButton);

	getContext().music->setPaused(true);

}

PauseState::~PauseState()
{
    getContext().music->setPaused(false);
}

void PauseState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());

    window.draw(backgroundShape);
    window.draw(m_pausedText);
    window.draw(m_GUIContainer);
}

bool PauseState::update(sf::Time)
{
    return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
    m_GUIContainer.handleEvent(event);
    return false;
}
