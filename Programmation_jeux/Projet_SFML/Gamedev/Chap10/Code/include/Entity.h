#ifndef ENTITY_H
#define ENTITY_H

#include <SceneNode.h>

/*
Entity permet de d�finir la vitesse et l'acc�l�ration d'un noeud(texture/image) de "SceneNode".
*/
class Entity : public SceneNode
{
public:
    explicit Entity(int hitpoints);

    void setVelocity(sf::Vector2f velocity); //donne la vitesse avec un vecteur
    void setVelocity(float vx, float vy); //donne la vitesse avec deux nombres
    void accelerate(sf::Vector2f velocity); //donne l'acc�l�ration avec un vecteur
	void accelerate(float vx, float vy); //donne l'acc�l�ration avec deux nombres
    sf::Vector2f getVelocity() const; //r�cup�re la valeur de la vitesse

    int getHitpoints() const;
    void setHitpoints(int points);
    void repair(int points);
    void damage(int points);
    void destroy();
    virtual void remove();
    virtual bool isDestroyed() const;


protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands); //d�place le noeud de la vitesse donn�.

private:
    sf::Vector2f m_velocity; //vecteur de vitesse
    int m_hitPoints; //point de vie de l'entit�
};

#endif // ENTITY_H
