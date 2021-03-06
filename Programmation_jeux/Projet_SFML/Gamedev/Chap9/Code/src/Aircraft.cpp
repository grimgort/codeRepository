#include <Aircraft.h>
#include <DataTables.h>
#include <Utility.h>
#include <Pickup.h>
#include <CommandQueue.h>
#include <SoundNode.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>

using namespace std::placeholders;

/*
R�cup�re les donn�es initiales des avions
*/
namespace
{
const std::vector<AircraftData> Table = initializeAircraftData();
}

/*
type = namespace contenant les noms des textures pouvant �tre associ�s aux Aircraft.
m_type = attribut de type
textures = Passage en r�f�rence de la class TextureHolder contenant l'ensemble des textures du jeux.
m_sprite = r�cup�re le sprite de la texture du TextureHolder d�finit par "type"(Eagle ou Raptor).
            "textures.get(toTextureID(type))" r�cup�re le pointeur de la class texture associ� � l'ID "type".
            La d�claration "sf::Sprite" de m_sprite associe m_sprite � la sous-classe de Texture nomm� Sprite.
*/
Aircraft::Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts)
    : Entity(Table[type].hitpoints)
    , m_type(type)
    , m_sprite(textures.get(Table[type].texture), Table[type].textureRect)
    , m_explosion(textures.get(Textures::Explosion))
    , m_fireCommand()
    , m_missileCommand()
    , m_fireCountDown(sf::Time::Zero)
    , m_isFiring(false)
    , m_isLaunchingMissile(false)
    , m_showExplosion(true)
    , m_playedExplosionSound(false)
    , m_spawnedPickup(false)
    , m_fireRateLevel(1)
    , m_spreadLevel(1)
    , m_missileAmmo(2)
    , m_dropPickupCommand()
    , m_travelledDistance(0.f)
    , m_directionIndex(0)
    , m_healthDisplay(nullptr)
    , m_missileDisplay(nullptr)
{
    m_explosion.setFrameSize(sf::Vector2i(256, 256));
    m_explosion.setNumFrames(16);
    m_explosion.setDuration(sf::seconds(1));

    centerOrigin(m_sprite);
    centerOrigin(m_explosion);

    m_fireCommand.category = Category::SceneAirLayer;
    m_fireCommand.action = [this, &textures] (SceneNode& node, sf::Time)
    {
        createBullets(node, textures);
    };

    m_missileCommand.category = Category::SceneAirLayer;
    m_missileCommand.action = [this, &textures] (SceneNode& node, sf::Time)
    {
        createProjectile(node, Projectile::Missile, 0.f, 0.5f, textures);
    };

    m_dropPickupCommand.category = Category::SceneAirLayer;
    m_dropPickupCommand.action = [this, & textures] (SceneNode& node, sf::Time)
    {
        createPickup(node, textures);
    };

    std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
    m_healthDisplay = healthDisplay.get();
    attachChild(std::move(healthDisplay));

    if (getCategory() == Category::PlayerAircraft)
    {
        std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
        missileDisplay->setPosition(0,70);
        m_missileDisplay = missileDisplay.get();
        attachChild(std::move(missileDisplay));
    }

    updateTexts();
}

/*
Permet de dessiner les textures d'aircraft sur l'�cran.

"RenderTarget" est une class de SFML qui permet de donner la fen�tre o� doit �tre dessiner la figure.
"RenderStates" est une class de SFML qui permet de donner les transformations du sprite (rotaton; shader, etc...)
*/
void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(isDestroyed() && m_showExplosion)
        target.draw(m_explosion, states);
    else
        target.draw(m_sprite, states);
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    updateTexts();
    updateRollAnimation();

    if (isDestroyed())
    {
        checkPickupDrop(commands);
        m_explosion.update(dt);

        if (!m_playedExplosionSound)
        {
            SoundEffect::ID soundEffect = (randomInt(2) == 0) ? SoundEffect::Explosion1 : SoundEffect::Explosion2;
            playLocalSound(commands, soundEffect);

            m_playedExplosionSound = true;
        }
        return;
    }

    checkProjectileLaunch(dt, commands);

    updateMovementPattern(dt);
    Entity::updateCurrent(dt, commands);
}

/*
Permet d'acc�der � la cat�gory du Aircraft en fonction de l'ID de la texture(Eagle ou Raptor).
Cela renvoit un entier diff�rent en fonction de la cat�gorie, qui est d�finit dans Cat�gory.h
*/
unsigned int Aircraft::getCategory() const
{
    if(isAllied())
        return Category::PlayerAircraft;
    else
        return Category::EnemyAircraft;

}

sf::FloatRect Aircraft::getBoundingRect() const
{
    return getWorldTransform().transformRect(m_sprite.getGlobalBounds());
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
    if (m_fireRateLevel < 10) ++m_fireRateLevel;
}

void Aircraft::increaseSpread()
{
    if (m_spreadLevel < 3) ++m_spreadLevel;
}

void Aircraft::collectMissiles(unsigned int count)
{
    m_missileAmmo += count;
}

void Aircraft::fire()
{
    if (Table[m_type].fireInterval != sf::Time::Zero)
        m_isFiring = true;
}

void Aircraft::launchMissile()
{
    if (m_missileAmmo > 0)
    {
        m_isLaunchingMissile = true;
        --m_missileAmmo;
    }
}

void Aircraft::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
    sf::Vector2f worldPosition =  getWorldPosition();

    Command command;
    command.category = Category::SoundEffect;
    command.action = derivedAction<SoundNode>(
        [effect, worldPosition] (SoundNode& node, sf::Time)
        {
            node.playSound(effect, worldPosition);
        });

    commands.push(command);
}


void Aircraft::updateMovementPattern(sf::Time dt)
{
    const std::vector<Direction>& directions = Table[m_type].directions;
    if (!directions.empty())
    {
        if (m_travelledDistance > directions[m_directionIndex].distance)
        {
            m_directionIndex = (m_directionIndex + 1) & directions.size();
            m_travelledDistance = 0.f;
        }

        float radians = toRadian(directions[m_directionIndex].angle + 90.f);
        float vx = getMaxSpeed() * std::cos(radians);
        float vy = getMaxSpeed() * std::sin(radians);

        setVelocity(vx, vy);

        m_travelledDistance += getMaxSpeed() * dt.asSeconds();
    }
}

void Aircraft::checkPickupDrop(CommandQueue& commands)
{
    if (!isAllied() && randomInt(1) == 0 && !m_spawnedPickup)
        commands.push(m_dropPickupCommand);

    m_spawnedPickup = true;
}

void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
    if(!isAllied()) fire();

    if (m_isFiring && m_fireCountDown <= sf::Time::Zero)
    {
        commands.push(m_fireCommand);
        playLocalSound(commands, isAllied() ? SoundEffect::AlliedGunfire : SoundEffect::EnemyGunfire);

        m_fireCountDown += Table[m_type].fireInterval / (m_fireRateLevel + 1.f);
        m_isFiring = false;
    }
    else if (m_fireCountDown > sf::Time::Zero)
    {
        m_fireCountDown -= dt;
        m_isFiring = false;
    }

    if (m_isLaunchingMissile)
    {
        commands.push(m_missileCommand);
        playLocalSound(commands, SoundEffect::LaunchMissile);
        m_isLaunchingMissile = false;
    }
}

void Aircraft::createBullets(SceneNode& node, const TextureHolder& textures) const
{
    Projectile::Type type = isAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;

    switch (m_spreadLevel)
    {
    case 1:
        createProjectile(node, type, 0.0f, 0.5f, textures);
        break;
    case 2:
        createProjectile(node, type, -0.33f, 0.33f, textures);
        createProjectile(node, type, +0.33f, 0.33f, textures);
        break;
    case 3:
        createProjectile(node, type, -0.5f, 0.33f, textures);
        createProjectile(node, type,  0.0f, 0.5f, textures);
        createProjectile(node, type, +0.5f, 0.33f, textures);
        break;
    }
}

void Aircraft::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
    std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

    sf::Vector2f offset(xOffset * m_sprite.getLocalBounds().width, yOffset * m_sprite.getGlobalBounds().height);
    sf::Vector2f velocity(0, projectile->getMaxSpeed());

    float sign = isAllied() ? -1.f : +1.f;
    projectile->setPosition(getWorldPosition() + offset * sign);
    projectile->setVelocity(velocity * sign);
    node.attachChild(std::move(projectile));
}

void Aircraft::createPickup(SceneNode& node, const TextureHolder& textures) const
{
    auto type = static_cast<Pickup::Type>(randomInt(Pickup::TypeCount));

    std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
    pickup->setPosition(getWorldPosition());
    pickup->setVelocity(0.f, 1.f);
    node.attachChild(std::move(pickup));
}

void Aircraft::updateTexts()
{
    if(isDestroyed())
        m_healthDisplay->setString("");
    else
        m_healthDisplay->setString(toString(getHitpoints()) + "HP");

    m_healthDisplay->setPosition(0.f, 50.f);
    m_healthDisplay->setRotation(-getRotation());

    if (m_missileDisplay)
    {
        if (m_missileAmmo == 0 || isDestroyed())
            m_missileDisplay->setString("");
        else
            m_missileDisplay->setString("M: " + toString(m_missileAmmo));
    }
}

void Aircraft::updateRollAnimation()
{
    if (Table[m_type].hasRollAnimation)
    {
        sf::IntRect textureRect = Table[m_type].textureRect;

        if (getVelocity().x < 0.f)
            textureRect.left += textureRect.width;

        else if (getVelocity().x > 0.f)
            textureRect.left += 2 * textureRect.width;

        m_sprite.setTextureRect(textureRect);
    }
}



