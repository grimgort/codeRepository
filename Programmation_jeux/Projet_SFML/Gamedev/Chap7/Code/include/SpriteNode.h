#ifndef SPRITENODE_H
#define SPRITENODE_H

#include <SceneNode.h>

#include <SFML/Graphics/Sprite.hpp>

/*
SpriteNode permet de tracer un sprite sans creer une entité.
Il permet donc de tracer le décors sans propriété autre que visuel
*/

class SpriteNode : public SceneNode
{
public:
    explicit SpriteNode(const sf::Texture& Texture);
    SpriteNode(const sf::Texture& Texture,const sf::IntRect& textureRect);
private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    //un sprite est une texture qui peut posséder différent argument comme un rectangle
    //définissant un cadre où doit être tracé la texture
    sf::Sprite m_sprite;
};

#endif // SPRITENODE_H
