#ifndef ENTITY_H
#define ENTITY_H

#include <SceneNode.h>

/*
Entity permet de définir la vitesse et l'accélération d'un noeud(texture/image) de "SceneNode".
*/
class Entity : public SceneNode
{
public:
    explicit Entity(int hitpoints);

    void setVelocity(sf::Vector2f velocity); //donne la vitesse avec un vecteur
    void setVelocity(float vx, float vy); //donne la vitesse avec deux nombres
    void accelerate(sf::Vector2f velocity); //donne l'accélération avec un vecteur
	void accelerate(float vx, float vy); //donne l'accélération avec deux nombres
    sf::Vector2f getVelocity() const; //récupére la valeur de la vitesse

    int getHitpoints() const;
    void setHitpoints(int points);
    void repair(int points);
    void damage(int points);
    void destroy();
    virtual void remove();
    virtual bool isDestroyed() const;


protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands); //déplace le noeud de la vitesse donné.

private:
    sf::Vector2f m_velocity; //vecteur de vitesse
    int m_hitPoints; //point de vie de l'entité
};

#endif // ENTITY_H
