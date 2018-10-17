#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <Entity.h>
#include <ResourceIdentifiers.h>

#include <SFML/Graphics/Sprite.hpp>

/*
Aircraft est la class associé aux textures d'avions Eagle et raptor. Elle hérite de la class Entity
qui définit la position et vitesse de l'image. Elle permet donc de spécifier des méthodes particulière aux Aircraft,
et nottament si l'avion est le joueur ou l'ennemie.
*/
class Aircraft : public Entity
{
public:
    enum Type
    {
        Eagle,
        Raptor,
    };

public:
    Aircraft(Type type,const TextureHolder& textures);
    virtual unsigned int getCategory() const;

private:
/*
Dessine la texture de l'avion sur la fenêtre target(RenderTarget) avec les éventuel transformation de "RenderStates".
*/
    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

private:
    Type m_type;
    sf::Sprite m_sprite;
};

#endif // AIRCRAFT_H
