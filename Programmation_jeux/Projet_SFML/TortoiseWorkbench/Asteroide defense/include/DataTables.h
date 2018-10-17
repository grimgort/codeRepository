#ifndef DATATABLES_H
#define DATATABLES_H

#include <ResourceIdentifiers.h>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include <functional>

class Aircraft;
class Base ;
class Asteroide ;

/////////////////////////////////////////////////
/// \file DataTables.h
/// \brief Contient les données des Entitées
/// \author Fred
/// \version 0.0
///
/// Les tables permettent de crer des objet eyant les même caractéristique
/// grâce à un identifiant. Ainsi, il est facile de construire un objet
/// et de modifier ses données de départ.
/////////////////////////////////////////////////





struct Direction
{
    Direction (float angle, float distance) :
        angle (angle),
        distance (distance)
    {
    }

    float angle;
    float distance;
};

struct AircraftData
{
    int hitpoints;
    float speed;
    sf::IntRect textureRect;
    Textures::ID texture;
    sf::Time fireInterval;
    std::vector<Direction> directions;
    bool hasRollAnimation;
};

struct ProjectileData
{
    int damage;
    float speed;
    Textures::ID texture;
    sf::IntRect textureRect;
    float distanceMax;

    int hitpoints;
};

struct PickupData
{
    std::function<void (Aircraft&) > action;
    Textures::ID texture;
    sf::IntRect textureRect;
};

struct ParticleData
{
    sf::Color color;
    sf::Time lifetime;
};

/////////////////////////////////////////////////
/// \struct Données des bases
///
/// Contient l'ensemble des données nécéssaire à la construction
/// d'une base : hitpoints(point de vie)
/////////////////////////////////////////////////
struct BaseData
{
    int hitpoints;
    Textures::ID texture;
};

struct AsteroideData
{
    int hitpoints;
    Textures::ID texture;
};

std::vector<AircraftData> initializeAircraftData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData> initializePickupData();
std::vector<ParticleData> initializeParticleData();
std::vector<BaseData> initializeBaseData();
std::vector<AsteroideData> initializeAsteroideData();

#endif // DATATABLES_H
