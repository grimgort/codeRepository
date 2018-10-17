#include "World.h"
#include <Projectile.h>
#include <Pickup.h>
#include <Foreach.hpp>
#include <TextNode.h>
#include <ParticleNode.h>
#include <SoundNode.h>
#include <NetworkNode.h>
#include <Utility.h>
#include <Base.h>

#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>


World::World (sf::RenderTarget& outputTarget,
              FontHolder& fonts, SoundPlayer& sounds,
              bool networked)
    : m_target (outputTarget)
    , m_sceneTexture()
    , m_worldView (outputTarget.getDefaultView())
    , m_viewTeam2 (outputTarget.getDefaultView())
    , m_textures()
    , m_fonts (fonts)
    , m_sounds (sounds)
    , m_sceneGraph()
    , m_sceneLayers()
    , m_worldBounds (0.f, 0.f, 2000.f, 5000.f)
    , m_spawnPosition (m_worldView.getSize().x / 2.f
                       , m_worldBounds.height - m_worldView.getSize().y / 2.f)
    , m_spawnPositionTeam2 (m_worldView.getSize().x / 2.f
                       , m_worldView.getSize().y / 2.f)
    , m_scrollSpeed (0.f)
    , m_scrollSpeedCompensation (1.f)
    , m_playerAircrafts()
    , m_enemySpawnPoints()
    , m_activeEnemies()
    , m_networkedWorld (networked)
    , m_networkNode (nullptr)
    , m_finishSprite (nullptr)
    , m_nbCutX (0)
    , m_nbCutY (0)
{
    m_sceneTexture.create (m_target.getSize().x,
                           m_target.getSize().y);
    loadTextures();
    buildScene();
    m_worldView.setCenter (m_spawnPosition);
    //On rajoute l'écran pour le J2 en local et on le tourne de 180°
    m_viewTeam2.setCenter (m_spawnPositionTeam2);
    m_viewTeam2.setRotation(180);

    grilleDeCollision();
}

void World::setWorldScrollCompensation (
    float compensation)
{
    m_scrollSpeedCompensation = compensation;
}

/*
    Met a jour la position des noeud et trace les texture associé.
*/

void World::update (sf::Time dt)
{
    //    toto =0;
    //Déplace la vue en fonction de la vitesse de l'avion.
    //Si on atteint de bord de la map, la vue reste statique et donc seul l'avion bouge.
    FOREACH (Aircraft * a, m_playerAircrafts)
    {
        //on récupére la vitesse et la position global de l'avion.
        sf::Vector2f velocity = a->getVelocity();
        sf::FloatRect position = a->getBoundingRect();
        int identifier = a->getIdentifier();

        if (identifier == 1)
        {
            //Déplacement en x
            m_worldView.move (velocity.x * dt.asSeconds() *
                              m_scrollSpeedCompensation, 0.f);

            if (m_worldView.getCenter().x - m_worldView.getSize().x / 2.f <
                    m_worldBounds.left)
            {

                m_worldView.setCenter (m_worldBounds.left + m_worldView.getSize().x /
                                       2.f, m_worldView.getCenter().y);
            }
            else if (m_worldView.getCenter().x + m_worldView.getSize().x / 2.f >
                     m_worldBounds.left + m_worldBounds.width)
            {

                m_worldView.setCenter (m_worldBounds.left + m_worldBounds.width -
                                       m_worldView.getSize().x / 2.f, m_worldView.getCenter().y);
            }

            //Déplacement en y
            m_worldView.move (0.f,
                              velocity.y * dt.asSeconds() * m_scrollSpeedCompensation);

            if (m_worldView.getCenter().y - m_worldView.getSize().y / 2.f <
                    m_worldBounds.top)
            {

                m_worldView.setCenter (m_worldView.getCenter().x,
                                       m_worldBounds.top + m_worldView.getSize().y / 2.f);
            }
            else if (m_worldView.getCenter().y + m_worldView.getSize().y / 2.f >
                     m_worldBounds.top + m_worldBounds.height)
            {

                m_worldView.setCenter (m_worldView.getCenter().x,
                                       m_worldBounds.top + m_worldBounds.height - m_worldView.getSize().y /
                                       2.f);
            }
        }
        // Correspond à l'avion 2 du split screen. A améliorer.
        else if (identifier == 2)
        {
            m_worldView.setViewport (sf::FloatRect (0, 0, 0.5f, 1));
            m_viewTeam2.setViewport (sf::FloatRect (0.5f, 0, 0.5f, 1));
            //Déplacement en x
            m_viewTeam2.move (velocity.x * dt.asSeconds() *
                              m_scrollSpeedCompensation, 0.f);

            if (m_viewTeam2.getCenter().x - m_viewTeam2.getSize().x / 2.f <
                    m_worldBounds.left)
            {

                m_viewTeam2.setCenter (m_worldBounds.left + m_viewTeam2.getSize().x /
                                       2.f, m_viewTeam2.getCenter().y);
            }
            else if (m_viewTeam2.getCenter().x + m_viewTeam2.getSize().x / 2.f >
                     m_worldBounds.left + m_worldBounds.width)
            {

                m_viewTeam2.setCenter (m_worldBounds.left + m_worldBounds.width -
                                       m_viewTeam2.getSize().x / 2.f, m_viewTeam2.getCenter().y);
            }

            //Déplacement en y
            m_viewTeam2.move (0.f,
                              velocity.y * dt.asSeconds() * m_scrollSpeedCompensation);

            if (m_viewTeam2.getCenter().y - m_viewTeam2.getSize().y / 2.f <
                    m_worldBounds.top)
            {

                m_viewTeam2.setCenter (m_viewTeam2.getCenter().x,
                                       m_worldBounds.top + m_viewTeam2.getSize().y / 2.f);
            }
            else if (m_viewTeam2.getCenter().y + m_viewTeam2.getSize().y / 2.f >
                     m_worldBounds.top + m_worldBounds.height)
            {

                m_viewTeam2.setCenter (m_viewTeam2.getCenter().x,
                                       m_worldBounds.top + m_worldBounds.height - m_viewTeam2.getSize().y /
                                       2.f);
            }
        }


    }
    //initialise la vitesse de l'avion du joueur à 0.
    //RQ:: sa vitesse est relative au monde(noeud enfant du noeud monde)
    FOREACH (Aircraft * a, m_playerAircrafts)
    a->setVelocity (0.f, 0.f);

    destroyEntitiesOutsideWorld();
    guideMissiles();

    //Tant que la liste des commande n'est pas vide(m_commandQueu pas vide)
    //on applique la fonction onCommand aux noeud.
    while (!m_commandQueue.isEmpty())
    {
        m_sceneGraph.onCommand (m_commandQueue.pop(), dt);
    }

    //Permet d'adapter le mouvement en fonction des commandes.
    //En particulier en cas de mouvement diagonal où l'on fiat la racide de 2
    adaptPlayerVelocity();
    handleCollisions();
    auto firstToRemove = std::remove_if (
                             m_playerAircrafts.begin(),
                             m_playerAircrafts.end(),
                             std::mem_fn (&Aircraft::isMarkedForRemoval));
    m_playerAircrafts.erase (firstToRemove,
                             m_playerAircrafts.end());
    m_sceneGraph.removeWrecks();
    spawnEnemies();
    m_sceneGraph.update (dt,
                         m_commandQueue); //on trace les sprite associé aux noeuds
    adaptPlayerPosition();
    updateSounds();
}

void World::draw()
{
    if (PostEffect::isSupported())
    {
        m_sceneTexture.clear();
        /*Applique la scéne à la deuxième vue pour le split screen*/
        m_sceneTexture.setView (m_viewTeam2);
        m_sceneTexture.draw (m_sceneGraph);
        /*Applique la scéne à la vue de base*/
        m_sceneTexture.setView (m_worldView);
        m_sceneTexture.draw (m_sceneGraph);


        m_sceneTexture.display();
        m_bloomEffect.apply (m_sceneTexture, m_target);

        //                m_sceneTexture.clear();
        //        m_sceneTexture.setView (m_viewTeam2);
        //        m_sceneTexture.draw (m_sceneGraph);
        //        m_sceneTexture.display();
    }
    else
    {
        /*Applique la scéne à la deuxième vue pour le split screen*/
        m_sceneTexture.setView (m_viewTeam2);
        m_sceneTexture.draw (m_sceneGraph);
        /*Applique la scéne à la vue de base*/
        m_sceneTexture.setView (m_worldView);
        m_sceneTexture.draw (m_sceneGraph);
    }
}

/*
    Récupére la liste des commandes en cours
*/
CommandQueue& World::getCommandQueue()
{
    return m_commandQueue;
}

Aircraft* World::getAircraft (int identifier)
const
{
    FOREACH (Aircraft * a, m_playerAircrafts)
    {
        if (a->getIdentifier() == identifier)
        {
            return a;
        }
    }
    return nullptr;
}

void World::removeAircraft (int identifier)
{
    Aircraft* aircraft = getAircraft (identifier);

    if (aircraft)
    {
        aircraft->destroy();
        m_playerAircrafts.erase (std::find (
                                     m_playerAircrafts.begin(),
                                     m_playerAircrafts.end(), aircraft));
    }
}

Aircraft* World::addAircraft (int identifier)
{
    std::unique_ptr<Aircraft> player (new Aircraft (
                                          Aircraft::Eagle, m_textures, m_fonts));
    player->setPosition (m_worldView.getCenter());
    player->setIdentifier (identifier);
    m_playerAircrafts.push_back (player.get());
    m_sceneLayers[UpperAir]->attachChild (std::move (
            player));
    return m_playerAircrafts.back();
}

void World::createPickup (sf::Vector2f position,
                          Pickup::Type type)
{
    std::unique_ptr<Pickup> pickup (new Pickup (type,
                                    m_textures));
    pickup->setPosition (position);
    pickup->setVelocity (0.f, 1.f);
    m_sceneLayers[UpperAir]->attachChild (std::move (
            pickup));
}

bool World::pollGameAction (GameActions::Action&
                            out)
{
    return m_networkNode->pollGameAction (out);
}

void World::setCurrentBattleFieldPosition (float lineY)
{
    m_worldView.setCenter (m_worldView.getCenter().x,
                           lineY - m_worldView.getSize().y / 2);
    m_spawnPosition.y = m_worldBounds.height;
}

void World::setWorldHeight (float height)
{
    m_worldBounds.height = height;
}

bool World::hasAlivePlayer() const
{
    return m_playerAircrafts.size() > 0;
}

bool World::hasPlayerReachedEnd() const
{
    if (Aircraft* aircraft = getAircraft (1))
    {
        return !m_worldBounds.contains (aircraft->getPosition());
    }
    else
    {
        return false;
    }
}

/*
    Charge les textures et les associe à leur enum de texture à l'initialisation du niveau pour délimiter le temps
    se chargement au début du jeux
*/
void World::loadTextures()
{
    m_textures.load (Textures::Entities,
                     "media/Textures/Entities.png");
    m_textures.load (Textures::Space,
                     "media/Textures/Space.png");
    m_textures.load (Textures::Explosion,
                     "media/Textures/Explosion.png");
    m_textures.load (Textures::Particle,
                     "media/Textures/Particle.png");
    m_textures.load (Textures::FinishLine,
                     "media/Textures/FinishLine.png");
    m_textures.load (Textures::Base,
                     "media/Textures/base.png");
    m_textures.load (Textures::AsteroideUn,
                     "media/Textures/asteroide1.png");
}

/*
    Permet d'adapter la position du joueur en fonction des bord du niveau (peut pas sortir de l'écran)
*/
void World::adaptPlayerPosition()
{
    //récupére les dimension de la fenêtre principal
    sf::FloatRect viewBounds = getViewBounds();
    sf::FloatRect viewBounds2 = getViewBoundsTeam2();
    //distance minimum entre les bord et le joueur
    const float borderDistance = 40.f;
    FOREACH (Aircraft * aircraft, m_playerAircrafts)
    {
        if (aircraft->getIdentifier() == 1)
        {

            //récupére la position de l'avion
            sf::Vector2f position = aircraft->getPosition();
            //on limite la position de l'avion aux bord de l'écran
            position.x = std::max (position.x,
                                   viewBounds.left + borderDistance);
            position.x = std::min (position.x,
                                   viewBounds.left + viewBounds.width -
                                   borderDistance);
            position.y = std::max (position.y,
                                   viewBounds.top + borderDistance);
            position.y = std::min (position.y,
                                   viewBounds.top + viewBounds.height -
                                   borderDistance);
            //on applique la nouvelle position
            aircraft->setPosition (position);
        }
        else if (aircraft->getIdentifier() == 2)
        {
            //récupére la position de l'avion
            sf::Vector2f position = aircraft->getPosition();
            //on limite la position de l'avion aux bord de l'écran
            position.x = std::max (position.x,
                                   viewBounds2.left + borderDistance);
            position.x = std::min (position.x,
                                   viewBounds2.left + viewBounds2.width -
                                   borderDistance);
            position.y = std::max (position.y,
                                   viewBounds2.top + borderDistance);
            position.y = std::min (position.y,
                                   viewBounds2.top + viewBounds2.height -
                                   borderDistance);
            //on applique lea nouvelle position
            aircraft->setPosition (position);
        }
    }
}

/*
    On adapte la vitesse du joueur en fonction de la vitesse de l'avion
*/
void World::adaptPlayerVelocity()
{
    FOREACH (Aircraft * aircraft, m_playerAircrafts)
    {
        //on récupére la vitesse de l'avion
        sf::Vector2f velocity = aircraft->getVelocity();

        //si la vitesse on a un mouvement diagonal on ajuste la distance.
        if (velocity.x != 0.f && velocity.y != 0.f)
        {
            aircraft->setVelocity (velocity / std::sqrt (2.f));
        }

        //on ajoute la vitesse de défilement du monde à la vitesse des commande clavier
        aircraft->accelerate (0.f, m_scrollSpeed);
    }
}

bool matchesCategories (SceneNode::Pair&
                        colliders, Category::Type type1,
                        Category::Type type2)
{
    unsigned int category1 =
        colliders.first->getCategory();
    unsigned int category2 =
        colliders.second->getCategory();

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
        std::swap (colliders.first, colliders.second);
        return true;
    }
    else
    {
        return false;
    }
}

void World::grilleDeCollision()
{
    m_nbCutY = 30; //Nombre de découpe en Y du monde
    m_nbCutX = 5; //Nombre de découpe en X du monde
    std::vector<float>
    leftRect; //conteneur des position gauche des rectangle découpé du world
    std::vector<float>
    topRect; //conteneur des position top des rectangle découpé du world
    float initialWidth = ceil (m_worldBounds.width /
                               m_nbCutX); //Width des rectangles initial
    float initialHeight = ceil (m_worldBounds.height /
                                m_nbCutY); //Height des rectangles initial
    leftRect.push_back (0.f);

    for (int i = 0; i < m_nbCutX; i++)
    {
        leftRect.push_back (ceil (leftRect[i] +
                                  m_worldBounds.left +  m_worldBounds.width /
                                  m_nbCutX));
    }

    topRect.push_back (0.f);

    for (int y = 0; y < m_nbCutY; y++)
    {
        topRect.push_back (ceil (topRect[y] +
                                 m_worldBounds.top + m_worldBounds.height /
                                 m_nbCutY));
    }

    for (int i = 0; i < m_nbCutX; i++)
    {
        for (int y = 0; y < m_nbCutY; y++)
        {
            sf::FloatRect rectTemp (leftRect[i], topRect[y],
                                    initialWidth, initialHeight);
            //cout de debug. A réfléchir pour améliorer la gestion des erreurs.
            //            std::cout << " i = " << i << " leftRect[i] = " << leftRect[i] << " topRect[y] = " << topRect[y]
            //                      << " initialWidth = " << initialWidth << " initialHeight = " << initialHeight << std::endl ;
            m_grilleDeCollision.push_back (rectTemp);
        }
    }
}

void World::handleCollisions()
{
    std::set<SceneNode::Pair> collisionPairs;
    std::multimap<int, SceneNode*>
    collisionListeToTest;

    m_sceneGraph.checkNodePosition (
        m_grilleDeCollision
        , collisionListeToTest
        , m_nbCutX
        , m_nbCutY);

    m_sceneGraph.checkSceneCollision (
        collisionListeToTest
        , collisionPairs
        , m_nbCutX
        , m_nbCutY);

    FOREACH (SceneNode::Pair pair, collisionPairs)
    {
        if (matchesCategories (pair
                               , Category::PlayerAircraft
                               , Category::EnemyAircraft))
        {
            auto& player = static_cast<Aircraft&>
                           (*pair.first);
            auto& enemy = static_cast<Aircraft&>
                          (*pair.second);
            player.damage (enemy.getHitpoints());
            enemy.destroy();
        }
        else if (matchesCategories (pair,
                                    Category::PlayerAircraft, Category::Pickup))
        {
            auto& player = static_cast<Aircraft&>
                           (*pair.first);
            auto& pickup = static_cast<Pickup&>
                           (*pair.second);
            pickup.apply (player);
            pickup.destroy();
            player.playLocalSound (m_commandQueue,
                                   SoundEffect::CollectPickup);
        }
        else if (matchesCategories (pair,
                                    Category::EnemyAircraft,
                                    Category::AlliedProjectile)
                 || matchesCategories (pair,
                                       Category::PlayerAircraft,
                                       Category::EnemyProjectile))

        {
            auto& aircraft = static_cast<Aircraft&>
                             (*pair.first);
            auto& projectile = static_cast<Projectile&>
                               (*pair.second);
            aircraft.damage (projectile.getDamage());
            projectile.destroy();
        }
        else if (matchesCategories (pair,
                                    Category::Base,
                                    Category::EnemyProjectile)
                 || matchesCategories (pair,
                                       Category::Base,
                                       Category::AlliedProjectile)
                 || matchesCategories (pair,
                                       Category::Base,
                                       Category::Asteroide))
        {
            auto& base = static_cast<Base&> (*pair.first);
            auto& projectile = static_cast<Projectile&> (*pair.second);
            base.damage (projectile.getDamage());
            projectile.destroy();
        }
        else if (matchesCategories (pair,
                                    Category::Base,
                                    Category::EnemyAircraft))
        {
            auto& base = static_cast<Base&> (*pair.first);
            auto& enemy = static_cast<Aircraft&> (*pair.second);
            base.damage (enemy.getHitpoints());
            enemy.destroy();
        }
        else if (matchesCategories (pair,
                                    Category::Asteroide,
                                    Category::EnemyAircraft)
                 || matchesCategories (pair,
                                       Category::Asteroide,
                                       Category::PlayerAircraft))
        {
            auto& aircraft = static_cast<Aircraft&>
                             (*pair.second);
            auto& asteroide = static_cast<Asteroide&>
                              (*pair.first);

            asteroide.damage (aircraft.getHitpoints());
            aircraft.damage (asteroide.getDamage());

        }
        else if (matchesCategories (pair,
                                    Category::Asteroide,
                                    Category::AlliedProjectile)
                 || matchesCategories (pair,
                                       Category::Asteroide,
                                       Category::EnemyProjectile))
        {
            auto& asteroide = static_cast<Asteroide&>
                              (*pair.first);
            auto& projectile = static_cast<Projectile&>
                               (*pair.second);

            asteroide.damage (projectile.getDamage());
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
        FOREACH (Aircraft * aircraft, m_playerAircrafts)
        listenerPosition += aircraft->getWorldPosition();
        listenerPosition /= static_cast<float>
                            (m_playerAircrafts.size());
    }

    m_sounds.setListenerPosition (listenerPosition);
    m_sounds.removeStoppedSounds();
}

void World::buildScene()
{
    // On créer un sceneNode par Layer que l'on stocke dans m_sceneLayers ; que l'on acroche à m_sceneGraph.
    // Si on met a jour m_sceneGraph, l'ensemble du jeux serat mis à jour.
    for (std::size_t i = 0; i < LayerCount; i++)
    {
        Category::Type category = (i == LowerAir) ?
                                  Category::SceneAirLayer : Category::None;
        SceneNode::Ptr layer (new SceneNode (category));
        m_sceneLayers[i] = layer.get();
        m_sceneGraph.attachChild (std::move (layer));
    }

    //Récupére la texture Space
    sf::Texture& SpaceTexture = m_textures.get (
                                     Textures::Space);
    SpaceTexture.setRepeated (true);
    float viewHeight = m_worldView.getSize().y;
    //Définit un rectangle avec les dimension correspondant au monde ou niveau
    sf::IntRect textureRect (m_worldBounds);
    textureRect.height += static_cast<int>
                          (viewHeight);
    //RQ:La classe SpriteNode va tracer la texture dans le rectangle lui étant associé.
    //Par conséquent, la texture desert va être répéter à l'intérieur du rectangle textureRect et non en dehors.
    //Définit le noeud correspondant au terrain
    std::unique_ptr<SpriteNode> SpaceSprite (
        new SpriteNode (SpaceTexture, textureRect));
    //place ce noeud en haut à gauche du monde ou niveau
    SpaceSprite->setPosition (m_worldBounds.left,
                               m_worldBounds.top);
    //Attache ce noeud à la scéne dans la partie bakground.
    m_sceneLayers[background]->attachChild (
        std::move (SpaceSprite));



    /* Rajoute la base 1  */
    std::unique_ptr<Base> baseUn (
        new Base (Base::BaseTypeUn, m_textures, m_fonts));
    baseUn->setPosition (0.f, 4650.f);
    m_sceneLayers[UpperAir]->attachChild (std::move (baseUn));

    /* Rajoute la base 2 */
    std::unique_ptr<Base> baseDeux (
        new Base (Base::BaseTypeUn, m_textures, m_fonts));
    baseDeux->setPosition (0.f, 0.f);
    m_sceneLayers[UpperAir]->attachChild (std::move (baseDeux));



    /* Rajoute la texture de la ligne d'arrivée*/
    //    sf::Texture& finishTexture = m_textures.get (
    //                                     Textures::FinishLine);
    //    std::unique_ptr<SpriteNode> finishSprite (
    //        new SpriteNode (finishTexture));
    //    finishSprite->setPosition (0.f, -76.f);
    //    m_finishSprite = finishSprite.get();
    //    m_sceneLayers[background]->attachChild (
    //        std::move (finishSprite));

    std::unique_ptr<ParticleNode> smokeNode (
        new ParticleNode (Particle::Smoke, m_textures));
    m_sceneLayers[LowerAir]->attachChild (std::move (
            smokeNode));

    std::unique_ptr<ParticleNode> propellantNode (
        new ParticleNode (Particle::Propellant,
                          m_textures));
    m_sceneLayers[LowerAir]->attachChild (std::move (
            propellantNode));

    std::unique_ptr<SoundNode> soundNode (
        new SoundNode (m_sounds));

    m_sceneGraph.attachChild (std::move (soundNode));

    if (m_networkedWorld)
    {
        std::unique_ptr<NetworkNode> networkNode (
            new NetworkNode());
        m_networkNode = networkNode.get();
        m_sceneGraph.attachChild (std::move (
                                      networkNode));
    }

    addEnemies();



}

// Fonction à supprimer ou modifier
void World::addEnemies()
{
    if (m_networkedWorld)
    {
        return;
    }


    for (float i = -10.f; i < 10.f; i++)
    {
        addEnemy (Aircraft::Avenger, i * 70, 1500.f);
    }

    for (float i = -10.f; i < 10.f; i++)
    {
        addEnemy (Aircraft::Avenger, i * 70, 4000.f);
    }

    for (float i = -10.f; i < 10.f; i++)
    {
        addEnemy (Aircraft::Avenger, i * 70, 2000.f);
    }

    //
    for (float i = -10.f; i < 10.f; i++)
    {
        addEnemy (Aircraft::Avenger, i * 70, 3000.f);
    }

    for (float i = -10.f; i < 10.f; i++)
    {
        addEnemy (Aircraft::Raptor, i * 70, 800.f);
    }


    sortEnemies();
}

void World::sortEnemies()
{
    std::sort (m_enemySpawnPoints.begin(),
               m_enemySpawnPoints.end(), [] (SpawnPoint lhs,
                       SpawnPoint rhs)
    {
        return lhs.y < rhs.y;
    });
}

void World::addEnemy (Aircraft::Type type,
                      float relX, float relY)
{
    SpawnPoint spawn (type, m_spawnPosition.x + relX,
                      m_spawnPosition.y - relY);
    m_enemySpawnPoints.push_back (spawn);
}

void World::spawnEnemies()
{
    while (!m_enemySpawnPoints.empty()
            && m_enemySpawnPoints.back().y >
            getBattlefieldBounds().top)
    {
        SpawnPoint spawn = m_enemySpawnPoints.back();
        std::unique_ptr<Aircraft> enemy (new Aircraft (
                                             spawn.type, m_textures, m_fonts));
        enemy->setPosition (spawn.x, spawn.y);
        enemy->setRotation (180.f);

        if (m_networkedWorld)
        {
            enemy->disablePickups();
        }

        m_sceneLayers[UpperAir]->attachChild (std::move (
                enemy));
        m_enemySpawnPoints.pop_back();
    }
}

void World::destroyEntitiesOutsideWorld()
{
    Command command;
    command.category = Category::Projectile |
                       Category::EnemyAircraft;
    command.action = derivedAction<Entity> ([this] (
            Entity & e, sf::Time)
    {
        if (!getBattlefieldBounds().intersects (
                    e.getBoundingRect()))
        {
            e.remove();
        }
    });
    m_commandQueue.push (command);
}

void World::guideMissiles()
{
    Command enemyCollector;
    enemyCollector.category = Category::EnemyAircraft;
    enemyCollector.action =
        derivedAction<Aircraft> ([this] (Aircraft & enemy,
                                         sf::Time)
    {
        if (!enemy.isDestroyed())
        {
            m_activeEnemies.push_back (&enemy);
        }
    });
    Command missileGuider;
    missileGuider.category =
        Category::AlliedProjectile;
    missileGuider.action =
        derivedAction<Projectile> ([this] (
                                       Projectile & missile, sf::Time)
    {
        if (!missile.isGuided())
        {
            return;
        }

        float minDistance =
            std::numeric_limits<float>::max();
        Aircraft* closestEnemy = nullptr;
        FOREACH (Aircraft * enemy, m_activeEnemies)
        {
            float enemyDistance = distance (missile, *enemy);

            if (enemyDistance < minDistance)
            {
                closestEnemy = enemy;
                minDistance = enemyDistance;
            }
        }

        if (closestEnemy)
        {
            missile.guideTowards (closestEnemy->getWorldPosition());
        }
    });
    m_commandQueue.push (enemyCollector);
    m_commandQueue.push (missileGuider);
    m_activeEnemies.clear();
}

sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect (m_worldView.getCenter() -
                          m_worldView.getSize() / 2.f,
                          m_worldView.getSize());
}

sf::FloatRect World::getViewBoundsTeam2() const
{
    return sf::FloatRect (m_viewTeam2.getCenter() -
                          m_viewTeam2.getSize() / 2.f,
                          m_viewTeam2.getSize());
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


