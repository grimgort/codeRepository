#include "GameState.h"

GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, m_world(*context.window, *context.fonts)
, m_player(*context.player)
{
    m_player.setMissionStatus(Player::MissionRunning);
}

void GameState::draw()
{
    m_world.draw();
}

bool GameState::update(sf::Time dt)
{
    m_world.update(dt);

    if(!m_world.hasAlivePlayer())
    {
        m_player.setMissionStatus(Player::MissionFailure);
        requestStackPush(States::GameOver);
    }
    else if(m_world.hasPlayerReachedEnd())
    {
        m_player.setMissionStatus(Player::MissionSucces);
        requestStackPush(States::GameOver);
    }

    CommandQueue& commands = m_world.getCommandQueue();
    m_player.handleRealtimeInput(commands);

    return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
    CommandQueue& commands = m_world.getCommandQueue();
    m_player.handleEvent(event, commands);

    if (event.type == sf::Event::KeyPressed  && event.key.code == sf::Keyboard::Escape)
        requestStackPush(States::Pause);

    return true;
}
