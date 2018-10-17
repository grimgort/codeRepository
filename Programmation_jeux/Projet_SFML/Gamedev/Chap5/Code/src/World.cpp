#include "World.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>


World::World(sf::RenderWindow& window ): m_window(window),
    m_worldView(window.getDefaultView()),
    m_worldBounds(0.f, 0.f, m_worldView.getSize().x, 2000.f),
    m_spawnPosition(m_worldView.getSize().x / 2.f, m_worldBounds.height - m_worldView.getSize().y / 2.f),
    m_playerAircraft(nullptr),
    m_scrollSpeed(-50.f),
    m_texture(),
    m_sceneGraph(),
    m_sceneLayers()
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

    //Tant que la liste des commande n'est pas vide(m_commandQueu pas vide)
    //on applique la fonction onCommand aux noeud.
    while (!m_commandQueue.isEmpty())
    {
        m_sceneGraph.onCommand(m_commandQueue.pop(),dt);
    }
    //Permet d'adapter le mouvement en fonction des commandes.
    //En particulier en cas de mouvement diagonal où l'on fiat la racide de 2
    adaptPlayerVelocity();

    m_sceneGraph.update(dt); //on trace les sprite associé aux noeuds
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
/*
Charge les textures et les associe à leur enum de texture à l'initialisation du niveau pour délimiter le temps
se chargement au début du jeux
*/
void World::loadTextures()
{
    m_texture.load(Textures::Eagle, "Media/Textures/Eagle.png");
    m_texture.load(Textures::Raptor, "Media/Textures/Raptor.png");
    m_texture.load(Textures::Desert, "Media/Textures/Desert.png");
}

void World::buildScene()
{
    // On créer un sceneNode par Layer que l'on stocke dans m_sceneLayers ; que l'on acroche à m_sceneGraph.
    // Si on met a jour m_sceneGraph, l'ensemble du jeux serat mis à jour.
    for (std::size_t i = 0; i < LayerCount; i++ )
    {
        SceneNode::Ptr layer(new SceneNode());
        m_sceneLayers[i] = layer.get();
        m_sceneGraph.attachChild(std::move(layer));
    }

    //Récupére la texture desert
    sf::Texture& texture = m_texture.get(Textures::Desert);
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
    std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Eagle, m_texture));
    //associe le noeud au joueur(pour les entrée clavier)
    m_playerAircraft = leader.get();
    //initialise la vitesse et la position du noeud
    m_playerAircraft->setPosition(m_spawnPosition);
    m_playerAircraft->setVelocity(40.f,m_scrollSpeed);
    //attache le neoud du joueur dans le conteneur m_sceneLayers associé à la partie Air
    m_sceneLayers[Air]->attachChild(std::move(leader));

    //Acroche les neoud escorte au joueur
    std::unique_ptr<Aircraft> leftEscort(new Aircraft(Aircraft::Raptor, m_texture));
    leftEscort->setPosition(-80.f, 50.f);
    m_playerAircraft->attachChild(std::move(leftEscort));

    std::unique_ptr<Aircraft> rightEscort(new Aircraft(Aircraft::Raptor, m_texture));
    rightEscort->setPosition(80.f, 50.f);
    m_playerAircraft->attachChild(std::move(rightEscort));
}

/*
Permet d'adapter la position du joueur en fonction des bord du niveau (peut pas sortir de l'écran)
*/
void World::adaptPlayerPosition()
{
    //récupére les dimension de la fenêtre principal
    sf::FloatRect viewBounds(m_worldView.getCenter() - m_worldView.getSize() / 2.f, m_worldView.getSize());
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
