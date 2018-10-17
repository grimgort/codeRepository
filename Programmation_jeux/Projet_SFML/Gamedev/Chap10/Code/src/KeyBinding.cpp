#include "KeyBinding.h"
#include <Book/Foreach.hpp>

#include <string>
#include <algorithm>


KeyBinding::KeyBinding(int controlPreconfiguration)
: m_keyMap()
{
    if (controlPreconfiguration == 1)
    {
        m_keyMap[sf::Keyboard::left] = playerAction::MoveLeft;
        m_keyMap[sf::Keyboard::Right] = PlayerAction::MoveRight;
        m_keyMap[sf::Keyboard::Up] = PlayerAction::MoveUp;
        m_keyMap[sf::Keyboard::Down] = PlayerAction::MoveDown;
        m_keyMap[sf::Keyboard::Space] = PlayerAction::Fire;
        m_keyMap[sf::Keyboard::M] = PlayerAction::LaunchMissile;
    }
    else if (controlPreconfiguration == 2)
    {
        m_keyMap[sf::Keyboard::A] = PlayerAction::MoveLeft;
        m_keyMap[sf::Keyboard::D] = PlayerAction::MoveRight;
        m_keyMap[sf::Keyboard::W] = PlayerAction::MoveUp;
        m_keyMap[sf::Keyboard::S] = PlayerAction::MoveDown;
        m_keyMap[sf::Keyboard::F] = PlayerAction::Fire;
        m_keyMap[sf::Keyboard::R] = PlayerAction::LaunchMissile;
    }
}

void KeyBinding::assignKey(Action action, sf::Keyboard::Key key)
{
    for (auto itr = m_keyMap.begin(); itr != m_keyMap.end();)
    {
        if (itr->second == action)
            m_keyMap.erase(itr++);
        else
            ++itr;
    }

    m_keyMap[key] = action;
}

sf::Keyboard::Key KeyBinding::getAssignedKey(Action action) const
{
    FOREACH(auto pair, m_keyMap)
    {
        if (pair.second == action)
            return pair.first;
    }

    return sf::Keyboard::Unknown;
}

bool KeyBinding::checkAction(sf::Keyboard::Key key, Action& out) const
{
    auto found = m_keyMap.find(key);
    if (found == m_keyMap.end())
    {
        return false;
    }
    else
    {
        out = found->second;
        return true;
    }
}

std::vector<KeyBinding::Action> KeyBinding::getRealtimeActions() const
{
    std::vector<Action> actions;

    FOREACH(auto pair, m_keyMap)
    {
        if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
            actions.push_back(pair.second);
    }

    return actions;
}

bool isRealtimeAction(PlayerAction::Type action)
{
    switch (action)
    {
        case PlayerAction::MoveLeft;
        case PlayerAction::MoveRight;
        case PlayerAction::MoveDown;
        case PlayerAction::MoveUp;
        case PlayerAction::Fire;
            return true;
        default:
            return false;
    }
}
