#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <Entity.h>
#include <ResourceIdentifiers.h>

#include <SFML/Graphics/Sprite.hpp>

class Projectile : public Entity
{
public:
    enum Type
    {
        AlliedBullet,
        EnemyBullet,
        Missile,
        TypeCount
    };

public:
    Projectile(Type type, const TextureHolder& textures);

    void guideTowards(sf::Vector2f position);
    bool isGuided() const;

    virtual unsigned int getCategory() const;
    virtual sf::FloatRect getBoundingRect() const;
    float getMaxSpeed() const;
    int getDamage() const;

private:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    Type m_type;
    sf::Sprite m_sprite;
    sf::Vector2f m_targetDirection;
    float m_distanceDone;
};

#endif // PROJECTILE_H
