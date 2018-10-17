#include <SpriteNode.h>

#include <SFML/Graphics/RenderTarget.hpp>

/*
Un constructeur définissant un sprite par une texture. Peut être tracé n'importe ou.
*/
SpriteNode::SpriteNode(const sf::Texture& texture)
: m_sprite(texture)
{
}

/*
Constructeur définissant un sprite avec une texture et un rectangle ou celle ci peut être tracé
*/
SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect)
: m_sprite(texture, textureRect)
{
}

/*
Trace le sprite (texture).
*/
void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
