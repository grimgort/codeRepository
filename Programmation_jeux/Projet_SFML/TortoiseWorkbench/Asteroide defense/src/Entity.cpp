#include <Entity.h>

#include <cassert>
#include <iostream>

Entity::Entity (int hitpoints)
    : m_velocity()
    , m_hitPoints (hitpoints)
    , m_positionCollision (-9999)
{
}


void Entity::setVelocity (sf::Vector2f velocity)
{
    m_velocity = velocity;
}

void Entity::setVelocity (float vx, float vy)
{
    m_velocity.x = vx;
    m_velocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
    return m_velocity;
}

void Entity::accelerate (sf::Vector2f velocity)
{
    m_velocity += velocity;
}

void Entity::accelerate (float vx, float vy)
{
    m_velocity.x += vx;
    m_velocity.y += vy;
}

int Entity::getHitpoints() const
{
    return m_hitPoints;
}

void Entity::setHitpoints (int points)
{
    assert (points > 0);
    m_hitPoints = points;
}

void Entity::repair (int points)
{
    assert (points > 0);
    m_hitPoints += points;
}

void Entity::damage (int points)
{
    assert (points > 0);
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

void Entity::updateCurrent (sf::Time dt
                            , CommandQueue&)
{
    move (m_velocity *
          dt.asSeconds()); //move inherit of transformable class. is the shortcut for getPosition()+offset()
}

// Calcul le numéro de la grille de collision
void Entity::checkNodePosition (const
                                std::vector<sf::FloatRect>
                                &virtualRectCollision
                                , std::multimap<int, SceneNode*>
                                &collisionListeToTest
                                , sf::Int32 nbCutX
                                , sf::Int32 nbCutY)
{
    /*
        Recalcule la position dans la grille de
        colllision si l'objet a bougé ou n'est pas
        initialisé
    */
    if (m_positionCollision == -9999)
    {
        int op (0);

        for (auto it = virtualRectCollision.cbegin()
                       ; it != virtualRectCollision.cend()
                ; ++it)
        {
            op += 1;

            // Regarde si la grille de collision contient le point. Plus optimisé que la fonction intersect.
            if (this->getBoundingRect().intersects (*it))
            {
                m_positionCollision = op;
                collisionListeToTest.insert (
                    std::pair<int, SceneNode*> (op, this));
                return;
            }
        }
    }
    else
    {
        /*
            Vérifie si l'objet est dans la grille du
            dessous ou dessus.
            Optimisable en rajoutant l'ensemble des
            cases adjacentes (sur les côté) mais
            nécessite de conaitre le nombre de
            case en X et Y.
        */
        for (int i = m_positionCollision - 1
                     ; i <= m_positionCollision + 1 ; ++i)
        {
            /*  Supprime les i négatif pour éviter
                d'avoir vector[-x](négatif) */
            if (i > 0)
            {
                // Regarde si la grille de collision contient le point. Plus optimisé que la fonction intersect.
                if (virtualRectCollision[i].intersects (
                            this->getBoundingRect()))
                {
                    m_positionCollision = i;
                    collisionListeToTest.insert (
                        std::pair<int, SceneNode*> (i, this));
                }
            }
        }

        for (int i = m_positionCollision - nbCutY - 1
                     ; i <= m_positionCollision - nbCutY + 1
                ; ++i)
        {
            if (i > 0)
            {
                // Regarde si la grille de collision contient le point. Plus optimisé que la fonction intersect.
                if (virtualRectCollision[i].intersects (
                            this->getBoundingRect()))
                {
                    collisionListeToTest.insert (
                        std::pair<int, SceneNode*> (i, this));
                    m_positionCollision = i;
                }
            }
        }

        // Regarde si la grille de collision contient le point. Plus optimisé que la fonction intersect.
        for (int i = m_positionCollision + nbCutY - 1
                     ; i <= m_positionCollision + nbCutY + 1
                ; ++i)
        {
            if (i > 0)
            {
                if (virtualRectCollision[i].intersects (
                            this->getBoundingRect()))
                {
                    collisionListeToTest.insert (
                        std::pair<int, SceneNode*> (i, this));
                    m_positionCollision = i;
                }
            }
        }
    }

    /*
        Sinon on a un problème car l'ensemble des entitées devrai avoir une position
    */
    //    else
    //    {
    //        std::cout << "ERROR = " << m_positionCollision << std::endl;
    //
    ////        collisionListeToTest.insert(std::pair<int,SceneNode*>(m_positionCollision,this));
    //    }
}

int Entity::getPositionCollision() const
{
    return m_positionCollision;
}
