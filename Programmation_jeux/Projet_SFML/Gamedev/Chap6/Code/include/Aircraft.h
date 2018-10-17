#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <Entity.h>
#include <ResourceIdentifiers.h>

#include <SFML/Graphics/Sprite.hpp>

/*
Aircraft est la class associ� aux textures d'avions Eagle et raptor. Elle h�rite de la class Entity
qui d�finit la position et vitesse de l'image. Elle permet donc de sp�cifier des m�thodes particuli�re aux Aircraft,
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
Dessine la texture de l'avion sur la fen�tre target(RenderTarget) avec les �ventuel transformation de "RenderStates".
*/
    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

private:
    Type m_type;
    sf::Sprite m_sprite;
};

#endif // AIRCRAFT_H
