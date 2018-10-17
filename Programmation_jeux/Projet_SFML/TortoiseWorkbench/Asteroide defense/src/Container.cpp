#include "Container.h"
#include <Foreach.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{

Container::Container() :
    m_children(),
    m_selectedChild (-1)
{
}

void Container::pack (Component::Ptr component)
{
    m_children.push_back (component);

    if (!hasSelection() && component->isSelectable())
    { select (m_children.size() - 1); }
}

bool Container::isSelectable() const
{
    return false;
}

void Container::handleEvent (const sf::Event& event)
{
    //si on a selectioner un enfant on lui donne l'event
    if (hasSelection() && m_children[m_selectedChild]->isActive())
    {
        m_children[m_selectedChild]->handleEvent (event);
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::Z
                || event.key.code == sf::Keyboard::Up)
        {
            selectPrevious();
        }
        else if (event.key.code == sf::Keyboard::S
                 || event.key.code == sf::Keyboard::Down)
        {
            selectNext();
        }
        else if (event.key.code == sf::Keyboard::Return
                 || event.key.code == sf::Keyboard::Space)
        {
            if (hasSelection())
            { m_children[m_selectedChild]->activate(); }
        }
    }
}

void Container::draw (sf::RenderTarget& target,
                      sf::RenderStates states) const
{
    states.transform *= getTransform();

    FOREACH (const Component::Ptr & child, m_children)
    target.draw (*child, states);
}

bool Container::hasSelection() const
{
    return m_selectedChild >= 0;
}

void Container::select (std::size_t index)
{
    if (m_children[index]->isSelectable())
    {
        if (hasSelection())
        { m_children[m_selectedChild]->deselect(); }

        m_children[index]->select();
        m_selectedChild = index;
    }
}

void Container::selectNext()
{
    if (!hasSelection()) { return; }

    // cherche le prochaine component selectable, (passe a travers if nÚcÚssaire)
    int next = m_selectedChild;

    do
    { next = (next + 1) % m_children.size(); }
    while (!m_children[next]->isSelectable());

    select (next);
}

void Container::selectPrevious()
{
    if (!hasSelection()) { return; }

    // cherche le precedent component selectable, (passe a travers if nÚcÚssaire)
    int prev = m_selectedChild;

    do
    { prev = (prev + m_children.size() - 1) % m_children.size() ; }
    while (!m_children[prev]->isSelectable());

    select (prev);
}








}
