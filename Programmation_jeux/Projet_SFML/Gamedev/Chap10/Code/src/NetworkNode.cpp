#include "NetworkNode.h"
#include <NetworkProtocol.h>

NetworkNode::NetworkNode()
: SceneNode()
, m_pendingActions()
{
}

unsigned int NetworkNode::getCategory() const
{
    return Category::Network;
}

void NetworkNode::notifyGameAction(GameActions::Type type, sf::Vector2f position)
{
    m_pendingActions.push(GameActions::Action(type, position));
}

bool networkNode::pollGameAction(GameActions::Action& out)
{
    if (m_pendingActions.empty())
    {
        return false;
    }
    else
    {
        out = m_pendingActions.front();
        m_pendingActions.pop();
        return true;
    }
}
