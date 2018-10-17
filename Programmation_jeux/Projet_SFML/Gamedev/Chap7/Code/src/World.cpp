#include "World.h"
#include <Projectile.h>
#include <Pickup.h>
#include <Foreach.hpp>
#include <TextNode.h>

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <limits>


World::World(sf::RenderWindow& window, FontHolder& fonts):
    m_window(window),
    m_worldView(window.getDefaultView()),
    m_fonts(fonts),
    m_textures(),
    m_sceneGraph(),
    m_sceneLayers(),
    m_worldBounds(0.f, 0.f, m_worldView.getSize().x, 2000.f),
    m_spawnPosition(m_worldView.getSize().x / 2.f, m_worldBounds.height - m_worldView.getSize().y / 2.f),
    m_scrollSpeed(-50.f),
    m_playerAircraft(nullptr),
    m_enemySpawnPoints(),
    m_activeEnemies()
{
    loadTextures();
    buildScene();
    m_worldView.setCenter(m_spawnPosition);
}

/*
Met a jour la position des noeud et trace les texture associé.
*/
void World::update(sf::Time dt)
{
    //fait avancer le monde à chaque frame
    m_worldView.move(0.f,m_scrollSpeed*dt.asSeconds());
    //initialise la vitesse de l'avion du joueur à 0.
    //RQ:: sa vitesse est relative au monde(noeud enfant du noeud monde)
    m_playerAircraft->setVelocity(0.f,0.f);

    destroyEntitiesOutsideView();
    guideMissiles();

    //Tant que la liste des commande n'est pas vide(m_commandQueu pas vide)
    //on applique la fonction onCommand aux noeud.
    while (!m_commandQueue.isEmpty())
    {
        m_sceneGraph.onCommand(m_commandQueue.pop(),dt);
    }
    //Permet d'adapter le mouvement en fonction des commandes.
    //En particulier en cas de mouvement diagonal où l'on fiat la racide de 2
    adaptPlayerVelocity();

    handleCollisions();

    m_sceneGraph.removeWrecks();
    spawnEnemies();

    m_sceneGraph.update(dt, m_commandQueue); //on trace les sprite associé aux noeuds
    adaptPlayerPosition();
}

void World::draw()
{
    m_window.setView(m_worldView);
    m_window.draw(m_sceneGraph);
}

/*
Récupére la liste des commandes en cours
*/
CommandQueue& World::getCommandQueue()
{
    return m_commandQueue;
}

bool World::hasAlivePlayer() const
{
    return !m_playerAircraft->isMarkedForRemoval();
}

bool World::hasPlayerReachedEnd() const
{
    return !m_worldBounds.contains(m_playerAircraft->getPosition());
}

/*
Charge les textures et les associe à leur enum de texture à l'initialisation du niveau pour délimiter le temps
se chargement au début du jeux
*/
void World::loadTextures()
{
    m_textures.load(Textures::Eagle, "Media/Textures/Eagle.png");
    m_textures.load(Textures::Raptor, "Media/Textures/Raptor.png");
    m_textures.load(Textures::Avenger, "Media/Textures/Avenger.png");
    m_textures.load(Textures::Desert, "Media/Textures/Desert.png");

    m_textures.load(Textures::Bullet, "Media/Textures/Bullet.png");
	m_textures.load(Textures::Missile, "Media/Textures/Missile.png");

	m_textures.load(Textures::HealthRefill, "Media/Textures/HealthRefill.png");
	m_textures.load(Textures::MissileRefill, "Media/Textures/MissileRefill.png");
	m_textures.load(Textures::FireSpread, "Media/Textures/FireSpread.png");
	m_textures.load(Textures::FireRate, "Media/Textures/FireRate.png");
}

/*
Permet d'adapter la position du joueur en fonction des bord du niveau (peut pas sortir de l'écran)
*/
void World::adaptPlayerPosition()
{

    //récupére les dimension de la fenêtre principal
    sf::FloatRect viewBounds = getViewBounds();
    //distance minimum entre les bord et le joueur
    const float borderDistance = 40.f;

    //récupére la position de l'avion
    sf::Vector2f position = m_playerAircraft->getPosition();
    //on limite la position de l'avion aux bord de l'écran
    position.x = std::max(position.x, viewBounds.left + borderDistance);
    position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
    position.y = std::max(position.y, viewBounds.top + borderDistance);
    position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
    //on applique lea nouvelle position
    m_playerAircraft->setPosition(position);
}

/*
On adapte la vitesse du joueur en fonction de la vitesse de l'avion
*/
void World::adaptPlayerVelocity()
{
    //on récupére la vitesse de l'avion
    sf::Vector2f velocity = m_playerAircraft->getVelocity();

    //si la vitesse on a un mouvement diagonal on ajuste la distance.
    if (velocity.x != 0.f && velocity.y != 0.f)
    {
        m_playerAircraft->setVelocity(velocity / std::sqrt(2.f));
    }

    //on ajoute la vitesse de défilement du monde à la vitesse des commande clavier
    m_playerAircraft->accelerate(0.f, m_scrollSpeed);
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

void World::handleCollisions()
{
    std::set<SceneNode::Pair> collisionPairs;
    m_sceneGraph.checkSceneCollision(m_sceneGraph, collisionPairs);

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

void World::buildScene()
{
    // On créer un sceneNode par Layer que l'on stocke dans m_sceneLayers ; que l'on acroche à m_sceneGraph.
    // Si on met a jour m_sceneGraph, l'ensemble du jeux serat mis à jour.
    for (std::size_t i = 0; i < LayerCount; i++ )
    {
        Category::Type category = (i ==Air) ? Category::SceneAirLayer : Category::None;
        SceneNode::Ptr layer(new SceneNode(category));
        m_sceneLayers[i] = layer.get();
        m_sceneGraph.attachChild(std::move(layer));
    }

    //Récupére la texture desert
    sf::Texture& texture = m_textures.get(Textures::Desert);
    //Définit un rectangle avec les dimension correspondant au monde ou niveau
    sf::IntRect textureRect(m_worldBounds);
    //Dit que la texture "texture" se répéte.
    texture.setRepeated(true);

    //RQ:La classe SpriteNode va tracer la texture dans le rectangle lui étant associé.
    //Par conséquent, la texture desert va être répéter à l'intérieur du rectangle textureRect et non en dehors.
    //Définit le noeud correspondant au terrain
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
    //place ce noeud en haut à gauche du monde ou niveau
    backgroundSprite->setPosition(m_worldBounds.left,m_worldBounds.top);
    //Attache ce noeud à la scéne dans la partie bakground.
    m_sceneLayers[background]->attachChild(std::move(backgroundSprite));

    //Crer un neoud leader comme étant une class aircraft
    std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, m_textures, m_fonts));
    //associe le noeud au joueur(pour les entrée clavier)
    m_playerAircraft = player.get();
    //initialise la vitesse et la position du noeud
    m_playerAircraft->setPosition(m_spawnPosition);
    m_playerAircraft->setVelocity(40.f,m_scrollSpeed);
    //attache le neoud du joueur dans le conteneur m_sceneLayers associé à la partie Air
    m_sceneLayers[Air]->attachChild(std::move(player));

    addEnemies();

    //Acroche les neoud escorte au joueur
//    std::unique_ptr<Aircraft> leftEscort(new Aircraft(Aircraft::Raptor, m_textures));
//    leftEscort->setPosition(-80.f, 50.f);
//    m_playerAircraft->attachChild(std::move(leftEscort));
//
//    std::unique_ptr<Aircraft> rightEscort(new Aircraft(Aircraft::Raptor, m_textures));
//    rightEscort->setPosition(80.f, 50.f);
//    m_playerAircraft->attachChild(std::move(rightEscort));
}

void World::addEnemies()
{
    addEnemy(Aircraft::Raptor, 0.f, 500.f);
    addEnemy(Aircraft::Raptor, 0.f, 1000.f);
    addEnemy(Aircraft::Raptor, +100.f, 1100.f);
    addEnemy(Aircraft::Raptor, -100.f, 1100.f);
    addEnemy(Aircraft::Avenger, -70.f, 1400.f);
    addEnemy(Aircraft::Avenger, -70.f, 1600.f);
    addEnemy(Aircraft::Avenger, 70.f, 1400.f);
    addEnemy(Aircraft::Avenger, -70.f, 1600.f);

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

        m_sceneLayers[Air]->attachChild(std::move(enemy));

        m_enemySpawnPoints.pop_back();
    }
}

void World::destroyEntitiesOutsideView()
{
    Command command;
    command.category = Category::Projectile | Category::EnemyAircraft;
    command.action = derivedAction<Entity>([this] (Entity& e, sf::Time)
    {
        if (!getBattlefieldBounds().intersects(e.getBoundingRect())) e.destroy();

    });

    m_commandQueue.push(command);
}

void World::guideMissiles()
{
    Command enemyCollector;
    enemyCollector.category = Category::EnemyAircraft;
    enemyCollector.action = derivedAction<Aircraft>([this] (Aircraft& enemy, sf::Time)
    {
        if (!enemy.isDestroyed()) m_activeEnemies.push_back(&enemy);
    });

    Command missileGuider;
    missileGuider.category = Category::AlliedProjectile;
    missileGuider.action = derivedAction<Projectile>([this] (Projectile& missile, sf::Time)
    {
        if (!missile.isGuided()) return;

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

        if (closestEnemy) missile.guideTowards(closestEnemy->getWorldPosition());
    });

    m_commandQueue.push(enemyCollector);
    m_commandQueue.push(missileGuider);
    m_activeEnemies.clear();

}

sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect(m_worldView.getCenter() - m_worldView.getSize() / 2.f, m_worldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
    sf::FloatRect bounds = getViewBounds();
    bounds.top -= 100.f;
    bounds.height += 100.f;

    return bounds;
}

