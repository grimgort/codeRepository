#include <LoadingState.h>
#include <Utility.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>



LoadingState::LoadingState(StateStack& stack, Context context)
: State(stack, context)
{
    sf::RenderWindow& window = *getContext().window;
    sf::Font& font = context.fonts->get(Fonts::Main);
    sf::Vector2f viewSize = window.getView().getSize();

    m_loadingText.setFont(font);
    m_loadingText.setString("Loading Resources");
    centerOrigin(m_loadingText);
    m_loadingText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f + 50.f);

    m_progressBarBackground.setFillColor(sf::Color::White);
    m_progressBarBackground.setSize((sf::Vector2f(viewSize.x - 20, 10)));
    m_progressBarBackground.setPosition(10, m_loadingText.getPosition().y +40);

    m_progressBar.setFillColor(sf::Color(100,100,100));
    m_progressBar.setSize(sf::Vector2f(200, 10));
    m_progressBar.setPosition(10, m_loadingText.getPosition().y + 40);

    setCompletion(0.f);

    m_loadingTask.execute();
}

void LoadingState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw (m_loadingText);
    window.draw(m_progressBarBackground);
    window.draw(m_progressBar);
}

bool LoadingState::update(sf::Time)
{
    if (m_loadingTask.isFinished())
    {
        requestStackPop();
        requestStackPush(States::Game);
    }
    else
    {
        setCompletion(m_loadingTask.getCompletion());
    }
    return true;
}

bool LoadingState::handleEvent(const sf::Event& event)
{
    return true;
}

void LoadingState::setCompletion(float percent)
{
    if (percent > 1.f)
        percent = 1.f;

    m_progressBar.setSize(sf::Vector2f(m_progressBarBackground.getSize().x * percent, m_progressBar.getSize().y));
}




