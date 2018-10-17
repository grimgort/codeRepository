#include <Aircraft.h>
#include <DataTables.h>
#include <Utility.h>
#include <Pickup.h>
#include <CommandQueue.h>
#include <SoundNode.h>
#include <NetworkNode.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <iostream>
#include <time.h>

using namespace std::placeholders;

/*
    Récupére les données initiales des avions
*/
namespace
{
const std::vector<AircraftData> Table = initializeAircraftData();
}

/*
    type = namespace contenant les noms des textures pouvant être associés aux Aircraft.
    m_type = attribut de type
    textures = Passage en référence de la class TextureHolder contenant l'ensemble des textures du jeux.
    m_sprite = récupére le sprite de la texture du TextureHolder définit par "type"(Eagle ou Raptor).
            "textures.get(toTextureID(type))" récupére le pointeur de la class texture associé à l'ID "type".
            La déclaration "sf::Sprite" de m_sprite associe m_sprite à la sous-classe de Texture nommé Sprite.
*/
Aircraft::Aircraft (Type type
                    , const TextureHolder& textures
                    , const FontHolder& fonts)
    : Entity (Table[type].hitpoints)
    , m_type (type)
    , m_sprite (textures.get (Table[type].texture),
                Table[type].textureRect)
    , m_explosion (textures.get (Textures::Explosion))
    , m_fireCommand()
    , m_missileCommand()
    , m_asteroideUnCommand()
    , m_fireCountDown (sf::Time::Zero)
    , m_isFiring (false)
    , m_isLaunchingMissile (false)
    , m_isLaunchingAsteroideUn (false)
    , m_showExplosion (true)
    , m_explosionBegan (false)
    , m_spawnedPickup (false)
    , m_pickupsEnabled (true)
    , m_fireRateLevel (1)
    , m_spreadLevel (1)
    , m_missileAmmo (2)
    , m_dropPickupCommand()
    , m_travelledDistance (0.f)
    , m_directionIndex (0)
    , m_missileDisplay (nullptr)
    , m_identifier (0)
{
    m_explosion.setFrameSize (sf::Vector2i (256, 256));
    m_explosion.setNumFrames (16);
    m_explosion.setDuration (sf::seconds (1));

    centerOrigin (m_sprite);
    centerOrigin (m_explosion);

    m_fireCommand.category = Category::SceneAirLayer;
    m_fireCommand.action = [this, &textures] (SceneNode & node, sf::Time)
    {
        createBullets (node, textures);
    };

    m_missileCommand.category = Category::SceneAirLayer;
    m_missileCommand.action = [this, &textures] (SceneNode & node,
                              sf::Time)
    {
        createProjectile (node, Projectile::Missile, 0.f, 0.5f, textures);
    };

    m_asteroideUnCommand.category = Category::SceneAirLayer;
    m_asteroideUnCommand.action = [this, &textures] (SceneNode & node,
                                  sf::Time)
    {
        createAsteroideUn (node, Asteroide::AsteroideUn, 0.f, 0.5f, textures);
    };

    m_dropPickupCommand.category = Category::SceneAirLayer;
    m_dropPickupCommand.action = [this, &textures] (SceneNode & node,
                                 sf::Time)
    {
        createPickup (node, textures);
    };

    std::unique_ptr<TextNode> healthDisplay (new TextNode (fonts, ""));
    m_healthDisplay = healthDisplay.get();
    attachChild (std::move (healthDisplay));

    if (getCategory() == Category::PlayerAircraft)
    {
        std::unique_ptr<TextNode> missileDisplay (new TextNode (fonts, ""));
        missileDisplay->setPosition (0, 70);
        m_missileDisplay = missileDisplay.get();
        attachChild (std::move (missileDisplay));
    }

    updateTexts();
}

int Aircraft::getMissileAmmo() const
{
    return m_missileAmmo;
}

void Aircraft::setMissileAmmo (int ammo)
{
    m_missileAmmo = ammo;
}

/*
    Permet de dessiner les textures d'aircraft sur l'écran.

    "RenderTarget" est une class de SFML qui permet de donner la fenêtre où doit être dessiner la figure.
    "RenderStates" est une class de SFML qui permet de donner les transformations du sprite (rotaton; shader, etc...)
*/
void Aircraft::drawCurrent (sf::RenderTarget& target
                            , sf::RenderStates states) const
{
    if (isDestroyed() && m_showExplosion)
    { target.draw (m_explosion, states); }
    else
    { target.draw (m_sprite, states); }
}

void Aircraft::disablePickups()
{
    m_pickupsEnabled = false;
}

void Aircraft::updateCurrent (sf::Time dt, CommandQueue& commands)
{
    updateTexts();
    updateRollAnimation();

    if (isDestroyed())
    {
        checkPickupDrop (commands);
        m_explosion.update (dt);

        if (!m_explosionBegan)
        {
            SoundEffect::ID soundEffect = (randomInt (2) == 0) ?
                                          SoundEffect::Explosion1 : SoundEffect::Explosion2;
            playLocalSound (commands, soundEffect);

            if (!isAllied())
            {
                sf::Vector2f position = getWorldPosition();

                Command command;
                command.category = Category::Network;
                command.action = derivedAction<NetworkNode> ([position] (
                                     NetworkNode & node, sf::Time)
                {
                    node.notifyGameAction (GameActions::EnemyExplode, position);
                });

                commands.push (command);
            }

            m_explosionBegan = true;
        }

        return;
    }

    checkProjectileLaunch (dt, commands);

    updateMovementPattern (dt);
    Entity::updateCurrent (dt, commands);
}

/*
    Permet d'accéder à la catégory du Aircraft en fonction de l'ID de la texture(Eagle ou Raptor).
    Cela renvoit un entier différent en fonction de la catégorie, qui est définit dans Catégory.h
*/
unsigned int Aircraft::getCategory() const
{
    if (isAllied())
    { return Category::PlayerAircraft; }
    else
    { return Category::EnemyAircraft; }

}

sf::FloatRect Aircraft::getBoundingRect() const
{
    return getWorldTransform().transformRect (m_sprite.getGlobalBounds());
}

bool Aircraft::isMarkedForRemoval() const
{
    return isDestroyed()
           && (m_explosion.isFinished() || !m_showExplosion);
}

void Aircraft::remove()
{
    Entity::remove();
    m_showExplosion = false;
}

bool Aircraft::isAllied() const
{
    return m_type == Eagle;
}

float Aircraft::getMaxSpeed() const
{
    return Table[m_type].speed;
}

void Aircraft::increaseFireRate()
{
    if (m_fireRateLevel < 10) { ++m_fireRateLevel; }
}

void Aircraft::increaseSpread()
{
    if (m_spreadLevel < 3) { ++m_spreadLevel; }
}

void Aircraft::collectMissiles (unsigned int count)
{
    m_missileAmmo += count;
}

void Aircraft::fire()
{
    if (Table[m_type].fireInterval != sf::Time::Zero)
    { m_isFiring = true; }
}

void Aircraft::launchMissile()
{
    if (m_missileAmmo > 0)
    {
        m_isLaunchingMissile = true;
        --m_missileAmmo;
    }
}

void Aircraft::launchAsteroideUn()
{
    m_isLaunchingAsteroideUn = true;
    /* Rajouter l'argent à enlever  */
}

void Aircraft::playLocalSound (CommandQueue& commands,
                               SoundEffect::ID effect)
{
    sf::Vector2f worldPosition =  getWorldPosition();

    Command command;
    command.category = Category::SoundEffect;
    command.action = derivedAction<SoundNode> (
                         [effect, worldPosition] (SoundNode & node, sf::Time)
    {
        node.playSound (effect, worldPosition);
    });

    commands.push (command);
}

int Aircraft::getIdentifier()
{
    return m_identifier;
}

void Aircraft::setIdentifier (int identifier)
{
    m_identifier = identifier;
}

void Aircraft::updateMovementPattern (sf::Time dt)
{
    const std::vector<Direction>& directions = Table[m_type].directions;

    if (!directions.empty())
    {
        if (m_travelledDistance > directions[m_directionIndex].distance)
        {
            m_directionIndex = (m_directionIndex + 1) % directions.size();
            m_travelledDistance = 0.f;
        }

        float radians = toRadian (directions[m_directionIndex].angle + 90.f);
        float vx = getMaxSpeed() * std::cos (radians);
        float vy = getMaxSpeed() * std::sin (radians);

        setVelocity (vx, vy);

        m_travelledDistance += getMaxSpeed() * dt.asSeconds();
    }
}

void Aircraft::checkPickupDrop (CommandQueue& commands)
{
    if (!isAllied() && randomInt (1) == 0 && !m_spawnedPickup
            && m_pickupsEnabled)
    { commands.push (m_dropPickupCommand); }

    m_spawnedPickup = true;
}

void Aircraft::checkProjectileLaunch (sf::Time dt,
                                      CommandQueue& commands)
{
    if (!isAllied()) { fire(); }

    if (m_isFiring && m_fireCountDown <= sf::Time::Zero)
    {
        commands.push (m_fireCommand);
        playLocalSound (commands,
                        isAllied() ? SoundEffect::AlliedGunfire : SoundEffect::EnemyGunfire);

        m_fireCountDown += Table[m_type].fireInterval / (m_fireRateLevel +
                           1.f);
        m_isFiring = false;
    }
    else if (m_fireCountDown > sf::Time::Zero)
    {
        m_fireCountDown -= dt;
        m_isFiring = false;
    }

    if (m_isLaunchingMissile)
    {
        commands.push (m_missileCommand);
        playLocalSound (commands, SoundEffect::LaunchMissile);
        m_isLaunchingMissile = false;
    }

    if (m_isLaunchingAsteroideUn)
    {
        commands.push (m_asteroideUnCommand);
        playLocalSound (commands, SoundEffect::LaunchAsteroideUn);
        m_isLaunchingAsteroideUn = false;
    }
}

void Aircraft::createBullets (SceneNode& node,
                              const TextureHolder& textures) const
{
    Projectile::Type type = isAllied() ? Projectile::AlliedBullet :
                            Projectile::EnemyBullet;

    switch (m_spreadLevel)
    {
    case 1:
        createProjectile (node, type, 0.0f, 0.5f, textures);
        break;

    case 2:
        createProjectile (node, type, -0.33f, 0.33f, textures);
        createProjectile (node, type, +0.33f, 0.33f, textures);
        break;

    case 3:
        createProjectile (node, type, -0.5f, 0.33f, textures);
        createProjectile (node, type,  0.0f, 0.5f, textures);
        createProjectile (node, type, +0.5f, 0.33f, textures);
        break;
    }
}

void Aircraft::createProjectile (SceneNode& node,
                                 Projectile::Type type, float xOffset, float yOffset,
                                 const TextureHolder& textures) const
{
    std::unique_ptr<Projectile> projectile (new Projectile (type,
                                            textures));

    sf::Vector2f offset (xOffset * m_sprite.getGlobalBounds().width,
                         yOffset * m_sprite.getGlobalBounds().height);
    sf::Vector2f velocity (0, projectile->getMaxSpeed());

    float sign = isAllied() ? -1.f : +1.f;
    projectile->setPosition (getWorldPosition() + offset * sign);
    projectile->setVelocity (velocity * sign);
    node.attachChild (std::move (projectile));
}


void Aircraft::createAsteroideUn (SceneNode& node,
                                  Projectile::Type type, float xOffset, float yOffset,
                                  const TextureHolder& textures) const
{
    std::unique_ptr<Asteroide> asteroide (new Asteroide (type, textures));


    //    srand(time(NULL));
    float nombre_aleatoire = rand() % (2000 - 0) + 0;


    sf::Vector2f offset (xOffset * m_sprite.getGlobalBounds().width,
                         yOffset * m_sprite.getGlobalBounds().height);
    sf::Vector2f velocity (0, asteroide->getMaxSpeed());
    float sign = isAllied() ? -1.f : +1.f;
    asteroide->setPosition (nombre_aleatoire, 4500.f);
    asteroide->setVelocity (velocity * sign);
    asteroide->changeScale (0.1f, 0.1f);
    node.attachChild (std::move (asteroide));
}

void Aircraft::createPickup (SceneNode& node,
                             const TextureHolder& textures) const
{
    auto type = static_cast<Pickup::Type> (randomInt (Pickup::TypeCount));

    std::unique_ptr<Pickup> pickup (new Pickup (type, textures));
    pickup->setPosition (getWorldPosition());
    pickup->setVelocity (0.f, 1.f);
    node.attachChild (std::move (pickup));
}

void Aircraft::updateTexts()
{
    if (isDestroyed())
    { m_healthDisplay->setString (""); }
    else
    { m_healthDisplay->setString (toString (getHitpoints()) + "HP"); }

    m_healthDisplay->setPosition (0.f, 50.f);
    m_healthDisplay->setRotation (-getRotation());

    if (m_missileDisplay)
    {
        if (m_missileAmmo == 0 || isDestroyed())
        { m_missileDisplay->setString (""); }
        else
        { m_missileDisplay->setString ("M: " + toString (m_missileAmmo)); }
    }
}

void Aircraft::updateRollAnimation()
{
    if (Table[m_type].hasRollAnimation)
    {
        sf::IntRect textureRect = Table[m_type].textureRect;

        if (getVelocity().x < 0.f)
        { textureRect.left += textureRect.width; }

        else if (getVelocity().x > 0.f)
        { textureRect.left += 2 * textureRect.width; }

        m_sprite.setTextureRect (textureRect);
    }
}
