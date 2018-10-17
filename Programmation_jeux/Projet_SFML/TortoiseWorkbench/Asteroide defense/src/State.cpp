#include "State.h"
#include "StateStack.h"

State::Context::Context (sf::RenderWindow& window,
                         TextureHolder& textures, FontHolder& fonts,
                         MusicPlayer& music, SoundPlayer& sounds, KeyBinding& keys1,
                         KeyBinding& keys2)
    : window (&window)
    , textures (&textures)
    , fonts (&fonts)
    , music (&music)
    , sounds (&sounds)
    , keys1 (&keys1)
    , keys2 (&keys2)
{
}

State::State (StateStack& stack, Context context)
    : m_stack (&stack)
    , m_context (context)
{
}

State::~State()
{
}

void State::requestStackPush (States::ID stateID)
{
    m_stack->pushState (stateID);
}

void State::requestStackPop()
{
    m_stack->popState();
}

void State::requestStateClear()
{
    m_stack->clearStates();
}

State::Context State::getContext() const
{
    return m_context;
}

void State::onActivate()
{
}

void State::onDestroy()
{
}

