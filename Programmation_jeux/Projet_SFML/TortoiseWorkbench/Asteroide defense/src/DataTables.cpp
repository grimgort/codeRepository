#include "DataTables.h"
#include <Aircraft.h>
#include <Projectile.h>
#include <Pickup.h>
#include <Particle.h>
#include <Base.h>
#include <Asteroide.h>

// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

//vecteur des données liées aux caractéristiques initial des aircraft. Il faut donc définir les attributs
//de la classe AircraftData.
std::vector<AircraftData> initializeAircraftData()
{
    //pas compris
    std::vector<AircraftData> data (Aircraft::TypeCount);

    //Avion Eagle
    data[Aircraft::Eagle].hitpoints = 10000;
    data[Aircraft::Eagle].speed = 1000.f;
    data[Aircraft::Eagle].fireInterval = sf::seconds (1);
    data[Aircraft::Eagle].texture = Textures::Entities;
    data[Aircraft::Eagle].textureRect = sf::IntRect (0, 0, 48, 64);
    data[Aircraft::Eagle].hasRollAnimation = true;

    //Avion Raptor
    data[Aircraft::Raptor].hitpoints = 20;
    data[Aircraft::Raptor].speed = 80.f;
    data[Aircraft::Raptor].texture = Textures::Entities;
    data[Aircraft::Raptor].textureRect = sf::IntRect (144, 0, 84, 64);
    data[Aircraft::Raptor].directions.push_back (Direction (+45.f, 80.f));
    data[Aircraft::Raptor].directions.push_back (Direction (-45.f, 160.f));
    data[Aircraft::Raptor].directions.push_back (Direction (+45.f, 80.f));
    data[Aircraft::Raptor].fireInterval = sf::Time::Zero;
    data[Aircraft::Raptor].hasRollAnimation = false;

    //Avion Avenger
    data[Aircraft::Avenger].hitpoints = 40;
    data[Aircraft::Avenger].speed = 50.f;
    data[Aircraft::Avenger].texture = Textures::Entities;
    data[Aircraft::Avenger].textureRect = sf::IntRect (228, 0, 60, 59);
    data[Aircraft::Avenger].directions.push_back (Direction (+45.f, 50.f));
    data[Aircraft::Avenger].directions.push_back (Direction (0.f, 50.f));
    data[Aircraft::Avenger].directions.push_back (Direction (-45.f, 100.f));
    data[Aircraft::Avenger].directions.push_back (Direction (0.f, 50.f));
    data[Aircraft::Avenger].directions.push_back (Direction (+45.f, 50.f));
    data[Aircraft::Avenger].fireInterval = sf::seconds (2);
    data[Aircraft::Avenger].hasRollAnimation = false;

    return data;
}


std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data (Projectile::TypeCount);

    data[Projectile::AlliedBullet].hitpoints = 1;
    data[Projectile::AlliedBullet].damage = 10;
    data[Projectile::AlliedBullet].speed = 500.f;
    data[Projectile::AlliedBullet].texture = Textures::Entities;
    data[Projectile::AlliedBullet].textureRect = sf::IntRect (175, 64, 3, 14);
    data[Projectile::AlliedBullet].distanceMax = 800.f;

    data[Projectile::EnemyBullet].hitpoints = 1;
    data[Projectile::EnemyBullet].damage = 10;
    data[Projectile::EnemyBullet].speed = 300.f;
    data[Projectile::EnemyBullet].texture = Textures::Entities;
    data[Projectile::EnemyBullet].textureRect = sf::IntRect (178, 64, 3, 14);
    data[Projectile::EnemyBullet].distanceMax = 800.f;

    data[Projectile::Missile].hitpoints = 1;
    data[Projectile::Missile].damage = 200;
    data[Projectile::Missile].speed = 150.f;
    data[Projectile::Missile].texture = Textures::Entities;
    data[Projectile::Missile].textureRect = sf::IntRect (160, 64, 15, 32);
    data[Projectile::Missile].distanceMax = -9999.f;

    data[Projectile::AsteroideUn].hitpoints = 20;
    data[Projectile::AsteroideUn].damage = 20;
    data[Projectile::AsteroideUn].speed = 150.f;
    data[Projectile::AsteroideUn].texture = Textures::AsteroideUn;
    data[Projectile::AsteroideUn].textureRect = sf::IntRect (0, 0, 500, 500);
    data[Projectile::AsteroideUn].distanceMax = -9999.f;

    return data;
}

std::vector<PickupData> initializePickupData()
{
    std::vector<PickupData> data (Pickup::TypeCount);

    data[Pickup::HealthRefill].texture = Textures::Entities;
    data[Pickup::HealthRefill].textureRect = sf::IntRect (0, 64, 40, 40);
    data[Pickup::HealthRefill].action = [] (Aircraft & a)
    {
        a.repair (25);
    };

    data[Pickup::MissileRefill].texture = Textures::Entities;
    data[Pickup::MissileRefill].textureRect = sf::IntRect (40, 64, 40, 40);
    data[Pickup::MissileRefill].action = std::bind (
            &Aircraft::collectMissiles, _1, 3);

    data[Pickup::FireSpread].texture = Textures::Entities;
    data[Pickup::FireSpread].textureRect = sf::IntRect (80, 64, 40, 40);
    data[Pickup::FireSpread].action = std::bind (
                                          &Aircraft::increaseSpread, _1);

    data[Pickup::FireRate].texture = Textures::Entities;
    data[Pickup::FireRate].textureRect = sf::IntRect (120, 64, 40, 40);
    data[Pickup::FireRate].action = std::bind (
                                        &Aircraft::increaseFireRate, _1);

    return data;
}

std::vector<ParticleData> initializeParticleData()
{
    std::vector<ParticleData> data (Particle::ParticleCount);

    data[Particle::Propellant].color = sf::Color (255, 255, 50);
    data[Particle::Propellant].lifetime = sf::seconds (0.6f);

    data[Particle::Smoke].color = sf::Color (50, 50, 50);
    data[Particle::Smoke].lifetime = sf::seconds (4.f);

    return data;
}

std::vector<BaseData> initializeBaseData()
{
    std::vector<BaseData> data (Base::TypeCount);
    //Premier type de base
    data[Base::BaseTypeUn].hitpoints = 10000;
    data[Base::BaseTypeUn].texture = Textures::Base;
    //   data[Base::BaseTypeUn].textureRect = sf::IntRect (228, 0, 60, 59);

    return data;
}

//std::vector<AsteroideData> initializeAsteroideData()
//{
//    std::vector<AsteroideData> data (Asteroide::TypeCount);
//    //Premier type de base
//    data[Asteroide::AsteroideUn].hitpoints = 20;
//    data[Asteroide::AsteroideUn].texture = Textures::AsteroideUn;
// //   data[Base::BaseTypeUn].textureRect = sf::IntRect (228, 0, 60, 59);
//
//
//    return data;
//}


