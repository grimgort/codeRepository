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
    Met à jour les menus en parcourant m_stack (qui contient tous les pointeurs des states)
    en utilisant la fonction update des states. applyPendingChanges() met à jour l'ordre de m_stack
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
    Récupére l'entrée clavier(event) qui déclanche un nouveau state
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
    rajoute un state dans le vecteur m_pendingList qui défini si un state doit être changé
*/
void StateStack::pushState (States::ID stateID)
{
    m_pendingList.push_back (PendingChange (Push, stateID));
}

/*
    Supprime le state de la pending list(à tuilisé aprés sa mise à jour)
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
    Retournbe le state recherché
*/
State::Ptr StateStack::createState (States::ID stateID)
{
    auto found = m_factories.find (stateID);
    assert (found != m_factories.end());

    return found->second();
}

/*
    met à jour les changement d'état de m_state et le libére de m_pendingList
    pour spécifier qu'il ne doit plus être appliqué.
*/
void StateStack::applyPendingChanges()
{
    FOREACH (PendingChange change, m_pendingList)
    {
        switch (change.action)
        {
        //récupére l'état de m_factorie et l'insert dans m_stack.
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







