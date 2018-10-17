#include "StateStack.h"
#include <Foreach.hpp>

#include <cassert>

StateStack::StateStack (State::Context context)
    : m_stack()
    , m_pendingList()
    , m_context (context)
    , m_factories()
{
}

/*
    Met � jour les menus en parcourant m_stack (qui contient tous les pointeurs des states)
    en utilisant la fonction update des states. applyPendingChanges() met � jour l'ordre de m_stack
*/
void StateStack::update (sf::Time dt)
{
    for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); itr++)
    {
        if (! (*itr)->update (dt))
        { break; }
    }

    applyPendingChanges();
}

/*
    Dessine les states de m_stack en utilisant le draw du state
*/
void StateStack::draw()
{
    // Draw all active states from bottom to top
    FOREACH (State::Ptr & state, m_stack)
    state->draw();
}

/*
    R�cup�re l'entr�e clavier(event) qui d�clanche un nouveau state
*/
void StateStack::handleEvent (const sf::Event& event)
{
    for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr)
    {
        if (! (*itr)->handleEvent (event))
        { break ; }
    }

    applyPendingChanges();
}

/*
    rajoute un state dans le vecteur m_pendingList qui d�fini si un state doit �tre chang�
*/
void StateStack::pushState (States::ID stateID)
{
    m_pendingList.push_back (PendingChange (Push, stateID));
}

/*
    Supprime le state de la pending list(� tuilis� apr�s sa mise � jour)
*/
void StateStack::popState()
{
    m_pendingList.push_back (PendingChange (Pop));
}

/*
    Supprime la ligne du state de la pending list
*/
void StateStack::clearStates()
{
    m_pendingList.push_back (PendingChange (Clear));
}

bool StateStack::isEmpty() const
{
    return m_stack.empty();
}

/*
    Retournbe le state recherch�
*/
State::Ptr StateStack::createState (States::ID stateID)
{
    auto found = m_factories.find (stateID);
    assert (found != m_factories.end());

    return found->second();
}

/*
    met � jour les changement d'�tat de m_state et le lib�re de m_pendingList
    pour sp�cifier qu'il ne doit plus �tre appliqu�.
*/
void StateStack::applyPendingChanges()
{
    FOREACH (PendingChange change, m_pendingList)
    {
        switch (change.action)
        {
        //r�cup�re l'�tat de m_factorie et l'insert dans m_stack.
        case Push:
            m_stack.push_back (createState (change.stateID));
            break;

        case Pop:
            m_stack.back()->onDestroy();
            m_stack.pop_back();

            if (!m_stack.empty())
            { m_stack.back()->onActivate(); }

            break;

        case Clear:
            FOREACH (State::Ptr & state, m_stack)
            state->onDestroy();

            m_stack.clear();
            break;
        }
    }
    m_pendingList.clear();
}

StateStack::PendingChange::PendingChange (Action action,
        States::ID stateID)
    : action (action)
    , stateID (stateID)
{
}







