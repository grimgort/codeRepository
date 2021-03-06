#include "World.h"
#include <Projectile.h>
#include <Pickup.h>
#include <Foreach.hpp>
#include <TextNode.h>
#include <ParticleNode.h>
#include <SoundNode.h>
#include <NetworkNode.h>
#include <Utility.h>

#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cmath>
#include <limits>


World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds, bool networked)
    :   m_target(outputTarget),
        m_sceneTexture(),
        m_worldView(outputTarget.getDefaultView()),
        m_textures(),
        m_fonts(fonts),
        m_sounds(sounds),
        m_sceneGraph(),
        m_sceneLayers(),
        m_worldBounds(0.f, 0.f, 2000.f, 5000.f),
        m_spawnPosition(m_worldView.getSize().x / 2.f, m_worldBounds.height - m_worldView.getSize().y / 2.f),
        m_scrollSpeed(0.f),
        m_scrollSpeedCompensation(1.f),
        m_playerAircrafts(),
        m_enemySpawnPoints(),
        m_activeEnemies(),
        m_networkedWorld(networked)
    , m_networkNode(nullptr)
    , m_finishSprite(nullptr)
{
    m_sceneTexture.create(m_target.getSize().x, m_target.getSize().y);

    loadTextures();
    buildScene();

    m_worldView.setCenter(m_spawnPosition);

    grilleDeCollision();
}

void World::setWorldScrollCompensation(float compensation)
{
    m_scrollSpeedCompensation = compensation;
}

/*
Met a jour la position des noeud et trace les texture associ�.
*/
void World::update(sf::Time dt)
{
    //D�place la vue en fonction de la vitesse de l'avion.
    //Si on atteint de bord de la map, la vue reste statique et donc seul l'avion bouge.
    FOREACH(Aircraft* a, m_playerAircrafts)
    {
        //on r�cup�re la vitesse et la position global de l'avion.
        sf::Vector2f velocity = a->getVelocity();
        sf::FloatRect position = a->getBoundingRect();
        //D�placement en x
        if (position.left + position.width + m_worldView.getSize().x / 2 >= m_worldBounds.width){}
        else if (position.left - m_worldView.getSize().x / 2 - position.width / 2 <= 0.f){}
        else
            m_worldView.move(velocity.x * dt.asSeconds() * m_scrollSpeedCompensation, 0.f);
        //D�placement en y
        if (position.top - m_worldView.getSize().y / 2 <= m_worldBounds.top){}
        else if (position.top + m_worldView.getSize().y / 2 + position.height >= m_worldBounds.height){}
        else
            m_worldView.move(0.f, velocity.y * dt.asSeconds() * m_scrollSpeedCompensation);
    }


    //initialise la vitesse de l'avion du joueur � 0.
    //RQ:: sa vitesse est relative au monde(noeud enfant du noeud monde)
    FOREACH(Aircraft* a, m_playerAircrafts)
    a->setVelocity(0.f, 0.f);

    destroyEntitiesOutsideWorld();
    guideMissiles();

    //Tant que la liste des commande n'est pas vide(m_commandQueu pas vide)
    //on applique la fonction onCommand aux noeud.
    while (!m_commandQueue.isEmpty())
        m_sceneGraph.onCommand(m_commandQueue.pop(),dt);

    //Permet d'adapter le mouvement en fonction des commandes.
    //En particulier en cas de mouvement diagonal o� l'on fiat la racide de 2
    adaptPlayerVelocity();

    handleCollisions();

    auto firstToRemove = std::remove_if(m_playerAircrafts.begin(), m_playerAircrafts.end(), std::mem_fn(&Aircraft::isMarkedForRemoval));
    m_playerAircrafts.erase(firstToRemove, m_playerAircrafts.end());

    m_sceneGraph.removeWrecks();
    spawnEnemies();

    m_sceneGraph.update(dt, m_commandQueue); //on trace les sprite associ� aux noeuds
    adaptPlayerPosition();

    updateSounds();
}

void World::draw()
{
    if (PostEffect::isSupported())
    {
        m_sceneTexture.clear();
        m_sceneTexture.setView(m_worldView);
        m_sceneTexture.draw(m_sceneGraph);
        m_sceneTexture.display();
        m_bloomEffect.apply(m_sceneTexture, m_target);
    }
    else
    {
        m_target.setView(m_worldView);
        m_target.draw(m_sceneGraph);
    }
}

/*
R�cup�re la liste des commandes en cours
*/
CommandQueue& World::getCommandQueue()
{
    return m_commandQueue;
}

Aircraft* World::getAircraft(int identifier) const
{
    FOREACH(Aircraft* a, m_playerAircrafts)
    {
        if (a->getIdentifier() == identifier)
            return a;
    }

    return nullptr;
}

void World::removeAircraft(int identifier)
{
    Aircraft* aircraft = getAircraft(identifier);
    if (aircraft)
    {
        aircraft->destroy();
        m_playerAircrafts.erase(std::find(m_playerAircrafts.begin(), m_playerAircrafts.end(), aircraft));
    }
}

Aircraft* World::addAircraft(int identifier)
{
    std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, m_textures, m_fonts));
    player->setPosition(m_worldView.getCenter());
    player->setIdentifier(identifier);

    m_playerAircrafts.push_back(player.get());
    m_sceneLayers[UpperAir]->attachChild(std::move(player));
    return m_playerAircrafts.back();
}

void World::createPickup(sf::Vector2f position, Pickup::Type type)
{
    std::unique_ptr<Pickup> pickup(new Pickup(type, m_textures));
    pickup->setPosition(position);
    pickup->setVelocity(0.f, 1.f);
    m_sceneLayers[UpperAir]->attachChild(std::move(pickup));
}

bool World::pollGameAction(GameActions::Action& out)
{
    return m_networkNode->pollGameAction(out);
}

void World::setCurrentBattleFieldPosition(float lineY)
{
    m_worldView.setCenter(m_worldView.getCenter().x, lineY - m_worldView.getSize().y / 2);
    m_spawnPosition.y = m_worldBounds.height;
}

void World::setWorldHeight(float height)
{
    m_worldBounds.height = height;
}

bool World::hasAlivePlayer() const
{
    return m_playerAircrafts.size() > 0;
}

bool World::hasPlayerReachedEnd() const
{
    if (Aircraft* aircraft = getAircraft(1))
        return !m_worldBounds.contains(aircraft->getPosition());
    else
        return false;
}

/*
Charge les textures et les associe � leur enum de texture � l'initialisation du niveau pour d�limiter le temps
se chargement au d�but du jeux
*/
void World::loadTextures()
{
    m_textures.load(Textures::Entities, "media/Textures/Entities.png");
    m_textures.load(Textures::Jungle, "media/Textures/Jungle.png");
    m_textures.load(Textures::Explosion, "media/Textures/Explosion.png");
    m_textures.load(Textures::Particle, "media/Textures/Particle.png");
    m_textures.load(Textures::FinishLine, "media/Textures/FinishLine.png");
}

/*
Permet d'adapter la position du joueur en fonction des bord du niveau (peut pas sortir de l'�cran)
*/
void World::adaptPlayerPosition()
{

    //r�cup�re les dimension de la fen�tre principal
    sf::FloatRect viewBounds = getViewBounds();
    //distance minimum entre les bord et le joueur
    const float borderDistance = 40.f;

    FOREACH(Aircraft* aircraft, m_playerAircrafts)
    {
        //r�cup�re la position de l'avion
        sf::Vector2f position = aircraft->getPosition();
        //on limite la position de l'avion aux bord de l'�cran
        position.x = std::max(position.x, viewBounds.left + borderDistance);
        position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
        position.y = std::max(position.y, viewBounds.top + borderDistance);
        position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
        //on applique lea nouvelle position
        aircraft->setPosition(position);
    }
}

/*
On adapte la vitesse du joueur en fonction de la vitesse de l'avion
*/
void World::adaptPlayerVelocity()
{
    FOREACH(Aircraft* aircraft, m_playerAircrafts)
    {
        //on r�cup�re la vitesse de l'avion
        sf::Vector2f velocity = aircraft->getVelocity();

        //si la vitesse on a un mouvement diagonal on ajuste la distance.
        if (velocity.x != 0.f && velocity.y != 0.f)
            aircraft->setVelocity(velocity / std::sqrt(2.f));

        //on ajoute la vitesse de d�filement du monde � la vitesse des commande clavier
        aircraft->accelerate(0.f, m_scrollSpeed);
    }
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
    unsigned int category1 = colliders.first->getCategory();
    unsigned int category2 = colliders.second->getCategory();

    if (type1 & category1 && type2 & category2)
    {
        return true;
    }
    else if (type1 & category2 && type2 & category2)
    {
        return true;
    }
    else if (type1 & category2 && type2 & category1)
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }
    else
    {
        return false;
    }
}

void World::grilleDeCollision()
{
    sf::Int32 nbCutY = 1; //Nombre de d�coupe en Y du monde
    sf::Int32 nbCutX = 1; //Nombre de d�coupe en X du monde
    std::vector<float> leftRect; //conteneur des position gauche des rectangle d�coup� du world
    std::vector<float> topRect; //conteneur des position top des rectangle d�coup� du world
    float initialWidth = ceil(m_worldBounds.width / nbCutX); //Width des rectangles initial
    float initialHeight = ceil(m_worldBounds.height / nbCutY); //Height des rectangles initial


    leftRect.push_back(0.f);
    for (int i = 0; i < nbCutX; i++)
    {
        leftRect.push_back(ceil(leftRect[i] +  m_worldBounds.left +  m_worldBounds.width / nbCutX));
    }

    topRect.push_back(0.f);
    for (int y = 0; y < nbCutY; y++)
    {
        topRect.push_back(ceil(topRect[y] +  m_worldBounds.top + m_worldBounds.height / nbCutY));
    }

    for (int i = 0; i < nbCutX; i++)
    {
        for (int y = 0; y < nbCutY; y++)
        {
            sf::FloatRect rectTemp(leftRect[i], topRect[y], initialWidth, initialHeight);
            m_grilleDeCollision.push_back(rectTemp);
        }
    }
}

void World::handleCollisions()
{
    std::set<SceneNode::Pair> collisionPairs;

//    m_sceneGraph.checkScenePosition(m_sceneGraph, rectCollision);
//    m_sceneGraph.checkSceneCollision(m_sceneGraph, collisionPairs);

    FOREACH(SceneNode::Pair pair, collisionPairs)
    {
        if (matchesCategories(pair, Category::PlayerAircraft, Category::EnemyAircraft))
        {
            auto& player = static_cast<Aircraft&>(*pair.first);
            auto& enemy = static_cast<Aircraft&>(*pair.second);

            player.damage(enemy.getHitpoints());
            enemy.destroy();
        }

        else if (matchesCategories(pair, Category::PlayerAircraft, Category::Pickup))
        {
            auto& player = static_cast<Aircraft&>(*pair.first);
            auto& pickup = static_cast<Pickup&>(*pair.second);

            pickup.apply(player);
            pickup.destroy();
            player.playLocalSound(m_commandQueue, SoundEffect::CollectPickup);
        }

        else if (matchesCategories(pair, Category::EnemyAircraft, Category::AlliedProjectile)
                 || matchesCategories(pair, Category::PlayerAircraft, Category::EnemyProjectile))
        {
            auto& aircraft = static_cast<Aircraft&>(*pair.first);
            auto& projectile = static_cast<Projectile&>(*pair.second);

            aircraft.damage(projectile.getDamage());
            projectile.destroy();
        }
    }
}

void World::updateSounds()
{
    sf::Vector2f listenerPosition;

    if (m_playerAircrafts.empty())
    {
        listenerPosition = m_worldView.getCenter();
    }
    else
    {
        FOREACH(Aircraft* aircraft, m_playerAircrafts)
        listenerPosition += aircraft->getWorldPosition();

        listenerPosition /= static_cast<float>(m_playerAircrafts.size());
    }
    m_sounds.setListenerPosition(listenerPosition);

    m_sounds.removeStoppedSounds();
}

void World::buildScene()
{
    // On cr�er un sceneNode par Layer que l'on stocke dans m_sceneLayers ; que l'on acroche � m_sceneGraph.
    // Si on met a jour m_sceneGraph, l'ensemble du jeux serat mis � jour.
    for (std::size_t i = 0; i < LayerCount; i++ )
    {
        Category::Type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;

        SceneNode::Ptr layer(new SceneNode(category));
        m_sceneLayers[i] = layer.get();

        m_sceneGraph.attachChild(std::move(layer));
    }

    //R�cup�re la texture desert
    sf::Texture& jungleTexture = m_textures.get(Textures::Jungle);
    jungleTexture.setRepeated(true);

    float viewHeight = m_worldView.getSize().y;
    //D�finit un rectangle avec les dimension correspondant au monde ou niveau
    sf::IntRect textureRect(m_worldBounds);
    textureRect.height += static_cast<int>(viewHeight);

    //RQ:La classe SpriteNode va tracer la texture dans le rectangle lui �tant associ�.
    //Par cons�quent, la texture desert va �tre r�p�ter � l'int�rieur du rectangle textureRect et non en dehors.
    //D�finit le noeud correspondant au terrain
    std::unique_ptr<SpriteNode> jungleSprite(new SpriteNode(jungleTexture, textureRect));
    //place ce noeud en haut � gauche du monde ou niveau
    jungleSprite->setPosition(m_worldBounds.left,m_worldBounds.top );
    //Attache ce noeud � la sc�ne dans la partie bakground.
    m_sceneLayers[background]->attachChild(std::move(jungleSprite));

    sf::Texture& finishTexture = m_textures.get(Textures::FinishLine);
    std::unique_ptr<SpriteNode> finishSprite(new SpriteNode(finishTexture));
    finishSprite->setPosition(0.f, -76.f);
    m_finishSprite = finishSprite.get();
    m_sceneLayers[background]->attachChild(std::move(finishSprite));

    std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(Particle::Smoke, m_textures));
    m_sceneLayers[LowerAir]->attachChild(std::move(smokeNode));

    std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(Particle::Propellant, m_textures));
    m_sceneLayers[LowerAir]->attachChild(std::move(propellantNode));

    std::unique_ptr<SoundNode> soundNode(new SoundNode(m_sounds));
    m_sceneGraph.attachChild(std::move(soundNode));

    if (m_networkedWorld)
    {
        std::unique_ptr<NetworkNode> networkNode(new NetworkNode());
        m_networkNode = networkNode.get();
        m_sceneGraph.attachChild(std::move(networkNode));
    }

    addEnemies();

}

void World::addEnemies()
{
    if (m_networkedWorld)
        return;

    for (float i = -10.f; i < 10.f; i++)
    {
        addEnemy(Aircraft::Avenger, i*70, 1500.f);
    }

    for (float i = -10.f; i < 10.f; i++)
    {
        addEnemy(Aircraft::Avenger, i*70, 4000.f);
    }

//    for (float i = -10.f; i < 10.f; i++)
//    {
//        addEnemy(Aircraft::Avenger, i*70, 2000.f);
//    }
//
//    for (float i = -10.f; i < 10.f; i++)
//    {
//        addEnemy(Aircraft::Avenger, i*70, 3000.f);
//    }


//    addEnemy(Aircraft::Raptor,    0.f,  500.f);
//    addEnemy(Aircraft::Raptor,    0.f, 1000.f);
//    addEnemy(Aircraft::Raptor, +100.f, 1150.f);
//    addEnemy(Aircraft::Raptor, -100.f, 1150.f);
    addEnemy(Aircraft::Avenger,  70.f, 1500.f);
//    addEnemy(Aircraft::Avenger, -70.f, 1500.f);
//    addEnemy(Aircraft::Avenger, -70.f, 1710.f);
//    addEnemy(Aircraft::Avenger,  70.f, 1700.f);
//    addEnemy(Aircraft::Avenger,  30.f, 1850.f);
//    addEnemy(Aircraft::Raptor,  300.f, 2200.f);
//    addEnemy(Aircraft::Raptor, -300.f, 2200.f);
//    addEnemy(Aircraft::Raptor,    0.f, 2200.f);
//    addEnemy(Aircraft::Raptor,    0.f, 2500.f);
//    addEnemy(Aircraft::Avenger,-300.f, 2700.f);
//    addEnemy(Aircraft::Avenger,-300.f, 2700.f);
//    addEnemy(Aircraft::Raptor,    0.f, 3000.f);
//    addEnemy(Aircraft::Raptor,  250.f, 3250.f);
//    addEnemy(Aircraft::Raptor, -250.f, 3250.f);
//    addEnemy(Aircraft::Avenger,   0.f, 3500.f);
//    addEnemy(Aircraft::Avenger,   0.f, 3700.f);
//    addEnemy(Aircraft::Raptor,    0.f, 3800.f);
//    addEnemy(Aircraft::Avenger,   0.f, 4000.f);
//    addEnemy(Aircraft::Avenger,-200.f, 4200.f);
//    addEnemy(Aircraft::Raptor,  200.f, 4200.f);
//    addEnemy(Aircraft::Raptor,    0.f, 4400.f);
//        addEnemy(Aircraft::Avenger,  0.f, 1500.f);
//    addEnemy(Aircraft::Avenger, 1.f, 1500.f);
//    addEnemy(Aircraft::Avenger, 2.f, 1710.f);
//    addEnemy(Aircraft::Avenger,  3.f, 1700.f);
//    addEnemy(Aircraft::Avenger,  4.f, 1850.f);
//        addEnemy(Aircraft::Avenger,  70.f, 1500.f);
//    addEnemy(Aircraft::Avenger, 5.f, 1500.f);
//    addEnemy(Aircraft::Avenger, 6.f, 1710.f);
//    addEnemy(Aircraft::Avenger,  7.f, 1700.f);
//    addEnemy(Aircraft::Avenger,  8.f, 1850.f);

    sortEnemies();
}

void World::sortEnemies()
{
    std::sort(m_enemySpawnPoints.begin(), m_enemySpawnPoints.end(), [] (SpawnPoint lhs, SpawnPoint rhs)
    {
        return lhs.y < rhs.y;
    });
}

void World::addEnemy(Aircraft::Type type, float relX, float relY)
{
    SpawnPoint spawn(type, m_spawnPosition.x + relX, m_spawnPosition.y - relY);
    m_enemySpawnPoints.push_back(spawn);
}

void World::spawnEnemies()
{
    while (!m_enemySpawnPoints.empty() && m_enemySpawnPoints.back().y > getBattlefieldBounds().top)
    {
        SpawnPoint spawn = m_enemySpawnPoints.back();

        std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.type, m_textures, m_fonts));
        enemy->setPosition(spawn.x, spawn.y);
        enemy->setRotation(180.f);
        if (m_networkedWorld)
            enemy->disablePickups();

        m_sceneLayers[UpperAir]->attachChild(std::move(enemy));

        m_enemySpawnPoints.pop_back();
    }
}

void World::destroyEntitiesOutsideWorld()
{
    Command command;
    command.category = Category::Projectile | Category::EnemyAircraft;
    command.action = derivedAction<Entity>([this] (Entity& e, sf::Time)
    {
        if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
            e.remove();
    });

    m_commandQueue.push(command);
}

void World::guideMissiles()
{
    Command enemyCollector;
    enemyCollector.category = Category::EnemyAircraft;
    enemyCollector.action = derivedAction<Aircraft>([this] (Aircraft& enemy, sf::Time)
    {
        if (!enemy.isDestroyed())
            m_activeEnemies.push_back(&enemy);
    });

    Command missileGuider;
    missileGuider.category = Category::AlliedProjectile;
    missileGuider.action = derivedAction<Projectile>([this] (Projectile& missile, sf::Time)
    {
        if (!missile.isGuided())
            return;

        float minDistance = std::numeric_limits<float>::max();
        Aircraft* closestEnemy = nullptr;

        FOREACH(Aircraft* enemy, m_activeEnemies)
        {
            float enemyDistance = distance(missile, *enemy);

            if (enemyDistance < minDistance)
            {
                closestEnemy = enemy;
                minDistance = enemyDistance;
            }
        }

        if (closestEnemy)
            missile.guideTowards(closestEnemy->getWorldPosition());
    });

    m_commandQueue.push(enemyCollector);
    m_commandQueue.push(missileGuider);
    m_activeEnemies.clear();

}

sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect(m_worldView.getCenter() - m_worldView.getSize() / 2.f, m_worldView.getSize());
}

sf::FloatRect World::getWorldBounds() const
{
    return m_worldBounds;
}

sf::FloatRect World::getBattlefieldBounds() const
{
    sf::FloatRect bounds = getWorldBounds();
    bounds.top -= 100.f;
    bounds.height += 100.f;

    return bounds;
}


