#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <Entity.h>
#include <Command.h>
#include <ResourceIdentifiers.h>
#include <Projectile.h>
#include <Asteroide.h>
#include <TextNode.h>
#include <Animation.h>

#include <SFML/Graphics/Sprite.hpp>

/*
    Aircraft est la classe associée aux textures d'avions Eagle et raptor. Elle hérite de la class Entity
    qui définit la position et vitesse de l'image. Elle permet donc de spécifier des méthodes particulière aux Aircraft,
    et nottament si l'avion est le joueur ou l'ennemi.
*/
class Aircraft : public Entity
{
public:
    enum Type
    {
        Eagle,
        Raptor,
        Avenger,
        TypeCount
    };

public:
    Aircraft (Type type, const TextureHolder& textures,
              const FontHolder& fonts);

    virtual unsigned int getCategory() const;
    virtual sf::FloatRect getBoundingRect() const;
    virtual void remove();
    virtual bool isMarkedForRemoval() const;
    bool isAllied() const;
    float getMaxSpeed() const;
    void disablePickups();

    void increaseFireRate();
    void increaseSpread();
    void collectMissiles (unsigned int count);

    void fire();
    void launchMissile();
    void launchAsteroideUn();
    void playLocalSound (CommandQueue& commands, SoundEffect::ID effect);
    int getIdentifier();
    void setIdentifier (int identifier);
    int getMissileAmmo() const;
    void setMissileAmmo (int ammo);

private:
    /*
        Dessine la texture de l'avion sur la fenêtre target(RenderTarget) avec les éventuel transformation de "RenderStates".
    */
    virtual void drawCurrent (sf::RenderTarget& target,
                              sf::RenderStates states) const;
    virtual void updateCurrent (sf::Time dt, CommandQueue& commands);
    void updateMovementPattern (sf::Time dt);
    void checkPickupDrop (CommandQueue& commands);
    void checkProjectileLaunch (sf::Time dt, CommandQueue& commands);

    void createBullets (SceneNode& node,
                        const TextureHolder& textures) const;
    void createProjectile (SceneNode& node, Projectile::Type type,
                           float XOffset, float yOffset, const TextureHolder& textures) const;
    void createPickup (SceneNode& node,
                       const TextureHolder& textures) const;
    void createAsteroideUn (SceneNode& node, Projectile::Type type,
                            float XOffset, float yOffset, const TextureHolder& textures) const;

    void updateTexts();
    void updateRollAnimation();

private:
    Type m_type;
    sf::Sprite m_sprite;
    Animation m_explosion;
    Command m_fireCommand;
    Command m_missileCommand;
    Command m_asteroideUnCommand;
    sf::Time m_fireCountDown;
    bool m_isFiring;
    bool m_isLaunchingMissile;
    bool m_isLaunchingAsteroideUn;
    bool m_showExplosion;
    bool m_explosionBegan;
    bool m_spawnedPickup;
    bool m_pickupsEnabled;

    int m_fireRateLevel;
    int m_spreadLevel;
    int m_missileAmmo;

    Command m_dropPickupCommand;
    float m_travelledDistance;
    std::size_t m_directionIndex;
    TextNode* m_healthDisplay;
    TextNode* m_missileDisplay;

    int m_identifier;
};

#endif // AIRCRAFT_H
