#include <Entity.h>

#include <cassert>

Entity::Entity(int hitpoints)
    : m_velocity(),
      m_hitPoints(hitpoints)
{
}


void Entity::setVelocity(sf::Vector2f velocity)
{
    m_velocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
    m_velocity.x = vx;
    m_velocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
    return m_velocity;
}

void Entity::accelerate(sf::Vector2f velocity)
{
    m_velocity += velocity;
}

void Entity::accelerate(float vx, float vy)
{
    m_velocity.x += vx;
    m_velocity.y += vy;
}

int Entity::getHitpoints() const
{
    return m_hitPoints;
}

void Entity::setHitpoints(int points)
{
    assert(points > 0);
    m_hitPoints = points;
}

void Entity::repair(int points)
{
    assert(points > 0);

    m_hitPoints += points;
}

void Entity::damage(int points)
{
    assert(points > 0);

    m_hitPoints -= points;
}

void Entity::destroy()
{
    m_hitPoints = 0;
}

void Entity::remove()
{
    destroy();
}

bool Entity::isDestroyed() const
{
    return m_hitPoints <= 0;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue&)
{
    move(m_velocity * dt.asSeconds()); //move inherit of transformable class. is the shortcut for getPosition()+offset()
}



