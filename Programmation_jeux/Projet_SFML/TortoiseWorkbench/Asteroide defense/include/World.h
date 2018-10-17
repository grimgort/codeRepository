#ifndef WORLD_H
#define WORLD_H

#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.h>
#include <SceneNode.h>
#include <SpriteNode.h>
#include <Aircraft.h>
#include <CommandQueue.h>
#include <Command.h>
#include <Pickup.h>
#include <BloomEffect.h>
#include <SoundPlayer.h>
#include <NetworkProtocol.h>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>



#include <array>
#include <queue>

/////////////////////////////////////////////////
/// \file World.h
/// \brief Est un niveau
/// \author Fred
/// \version 0.0
///
///
/////////////////////////////////////////////////


namespace sf
{
class RenderTarget;
}

class NetworkNode;

/////////////////////////////////////////////////
/// \class World
/// \brief Classe representant le niveau
///
///  La classe contient l'ensemble des textures, des entitées, etc...
///  Lorsque l'on appui sur les bouton play on créer cette classe.
/////////////////////////////////////////////////
class World : private sf::NonCopyable
{
public:
    /////////////////////////////////////////////////
    /// \brief Constructeur
    ///
    /// \param outputTarget : Fenêtre de l'application
    /// \param fonts : Conteneur des fonts du texte
    /// \param sounds : Class qui joue et contient les sons
    /// \param networked : Défini si le jeux est en ligne ou local
    ///
    /// Met en place le monde en chargant l'ensemble des texture de
    /// départ. Définie la position de la vue de départ.
    /////////////////////////////////////////////////
    explicit World (sf::RenderTarget& outputTarget, FontHolder& fonts,
                    SoundPlayer& sounds, bool networked = false);

    /////////////////////////////////////////////////
    /// \brief Met à jour l'ensemble des noeuds du world
    ///
    /// \param dt : Pas de temps du jeux
    ///
    /// Permet d'ajuster la vue des joueurs (2max).
    /// Détruit les entitées en dehors du monde.
    /// Guide les missiles.
    /// applique les actions des commandes.
    /// Lance le calcul des collisions.
    /// Met a jour les sons.
    /////////////////////////////////////////////////
    void update (sf::Time dt);

    /////////////////////////////////////////////////
    /// \brief Trace les vues et les textures associés
    ///
    /// Applique le bloom effect si la carte graphique le supporte.
    /////////////////////////////////////////////////
    void draw();

    /////////////////////////////////////////////////
    /// \brief Récupére le rectangle définissant la vue 1
    ///
    /// \return FloatRect de la vue 1
    /////////////////////////////////////////////////
    sf::FloatRect getViewBounds() const;

    /////////////////////////////////////////////////
    /// \brief Récupére le rectangle définissant la vue 2
    ///
    /// \return FloatRect de la vue 2
    /////////////////////////////////////////////////
    sf::FloatRect getViewBoundsTeam2() const;

    /////////////////////////////////////////////////
    /// \brief Récupére la liste des commandes en cours
    ///
    /// \return La référence de la liste des commandes en cours
    /////////////////////////////////////////////////
    CommandQueue& getCommandQueue();

    /////////////////////////////////////////////////
    /// \brief Ajoute un avion de type joueur
    ///
    /// \param identifier : identifiant de l'avion créer
    /// \return Pointeur de l'avion créer
    ///
    /// Créer un pointeur unique d'un avion joueur et défini sa position,
    /// son identifiant et son noeud pére.
    /////////////////////////////////////////////////
    Aircraft* addAircraft (int identifier);

    /////////////////////////////////////////////////
    /// \brief Détruit un avion de type joueur reconnu par son identifiant
    ///
    /// \param identifier : identifiant de l'avion créer
    /////////////////////////////////////////////////
    void removeAircraft (int identifier);

    /////////////////////////////////////////////////
    /// \brief A compléter
    ///
    /// \param lineY : position en y
    ///
    /// Utilisé pour la partie network.
    /////////////////////////////////////////////////
    void setCurrentBattleFieldPosition (float lineY);

    /////////////////////////////////////////////////
    /// \brief Définie la hauteur du niveau
    ///
    /// \param height : Hauteur choisit
    ///
    /// Utilisé pour la partie network.
    /////////////////////////////////////////////////
    void setWorldHeight (float height);

    /////////////////////////////////////////////////
    /// \brief Ajoute des avions ennemies
    ///
    /// \param type : type de l'avion a créer
    /// \param relX : spawn en X de l'avion
    /// \param relY : spawn en Y de l'avion
    ///
    /////////////////////////////////////////////////
    void addEnemy (Aircraft::Type type, float relX, float relY);

    /////////////////////////////////////////////////
    /// \brief Trie l'ordre des avions ennemies à créer
    ///
    /// obsolette dans la version actuel car les avions
    /// ennemies vont disparaitre
    /////////////////////////////////////////////////
    void sortEnemies();

    /////////////////////////////////////////////////
    /// \brief Vérifie que des avion joueurs sont encore présent
    ///
    /// \return Vrai si il reste au moin un joueur vivant
    /////////////////////////////////////////////////
    bool hasAlivePlayer() const;

    /////////////////////////////////////////////////
    /// \brief Vérifie si un joueur a atteint la ligne d'arrivé
    ///
    /// \return vrai si la ligne d'arrivé est atteinte
    ///
    /// Obsolette car plus de ligne d'arrivé
    /////////////////////////////////////////////////
    bool hasPlayerReachedEnd() const;

    /////////////////////////////////////////////////
    /// \brief Définit la scroll compensation (network)
    ///
    /// \param compensation : valeur de la compensation
    /////////////////////////////////////////////////
    void setWorldScrollCompensation (float compensation);

    /////////////////////////////////////////////////
    /// \brief Récupére le pointeur de l'avion joueur
    ///
    /// \param identifier : identifiant de l'avion joueur à récupérer
    /// \return Pointeur de l'avion
    /////////////////////////////////////////////////
    Aircraft* getAircraft (int identifier) const;

    /////////////////////////////////////////////////
    /// \brief Récupére les bordures du monde
    ///
    /// \return : le rectangle des bordure de l'écran.
    /////////////////////////////////////////////////
    sf::FloatRect getBattlefieldBounds() const;

    /////////////////////////////////////////////////
    /// \brief Récupére les dimension de la carte world
    ///
    /// \return Le rectangle du monde
    /////////////////////////////////////////////////
    sf::FloatRect getWorldBounds() const;

    /////////////////////////////////////////////////
    /// \brief Créer un drop de bonus (pickup)
    ///
    /// \param position : Vecteur définissant la position du drop
    /// \param type : Type du pickup (vie, dégat, etc...)
    /////////////////////////////////////////////////
    void createPickup (sf::Vector2f position, Pickup::Type type);

    /////////////////////////////////////////////////
    /// \brief A définir (network)
    ///
    /// \param out : ??
    /////////////////////////////////////////////////
    bool pollGameAction (GameActions::Action& out);

private:
    /////////////////////////////////////////////////
    /// \brief Charge les textures à afficher dans le monde
    ///
    /// Les nouvelles textures doivent être ajouter ici
    /////////////////////////////////////////////////
    void loadTextures();

    /////////////////////////////////////////////////
    /// \brief Adapte la position de l'avion en fonction des bordures du monde
    ///
    /// Evite que l'avion joueur dépasse les bordures du jeux.
    ///
    /////////////////////////////////////////////////
    void adaptPlayerPosition();

    /////////////////////////////////////////////////
    /// \brief adapte la vitesse de l'avion
    ///
    /// Ajuste la vitesse de l'avion pour les trajectoires diagonal.
    /////////////////////////////////////////////////
    void adaptPlayerVelocity();

    /////////////////////////////////////////////////
    /// \brief Calcul la grille de collision
    ///
    /// Calcul les dimensions des rectangles en fonction du nombre
    /// de coupe en X(m_nbCutX) et Y(m_nbCutY) définit par le dévelloper.
    /// La méthode compléte donc le vecteur m_grilleDeCollision qui
    /// contient l'ensemble des rectangle de la grille de collision.
    ///
    /////////////////////////////////////////////////
    void grilleDeCollision();

    /////////////////////////////////////////////////
    /// \brief Déclenche le calcul des collision
    ///
    /// Récupére la grille de collision et demande la position des
    /// noeud dans la grille, regarde si les noeud rentre en collision
    /// et applique l'effet des collision.
    ///
    /// Idée : Il est peut êter intérréssant d'en faire une classe (héritant
    /// du world???)
    ///
    /////////////////////////////////////////////////
    void handleCollisions();

    /////////////////////////////////////////////////
    /// \brief Met à joueur le son joué en fonction de la position du joueur
    ///
    /// Met a jour le son joué en fonction de la position de l'avion joueur
    /////////////////////////////////////////////////
    void updateSounds();

    /////////////////////////////////////////////////
    /// \brief Construit les objet de la scene à l'état initial
    ///
    /// Permet de construire l'ensemble des objet de la scene à l'état
    /// initial. C'est donc ici qu'il faut rajouter les objets qui doivent
    /// être construit au début de la scene. Elle est donc invoquer
    /// dans le constructeur de la classe World.
    /////////////////////////////////////////////////
    void buildScene();

    /////////////////////////////////////////////////
    /// \brief Ajoute la totalité des avions ennemies
    ///
    /// Les avions ennemie à créer sont a rajouter ici
    /////////////////////////////////////////////////
    void addEnemies();

    /////////////////////////////////////////////////
    /// \brief Ajoute les noeuds ennemies à la scene
    ///
    /// fonction secondaire à addEnemies.
    /////////////////////////////////////////////////
    void spawnEnemies();

    /////////////////////////////////////////////////
    /// \brief Détruit informatiquement les entité qui sont en-dehors du monde
    ///
    /////////////////////////////////////////////////
    void destroyEntitiesOutsideWorld();

    /////////////////////////////////////////////////
    /// \brief Guide les missiles vers l'ennemies le plus proche
    ///
    ///
    /////////////////////////////////////////////////
    void guideMissiles();

private:
    ///< Conteneur des ensembles de noeuds
    enum Layer
    {
        background,
        LowerAir,
        UpperAir,
        LayerCount
    };

    struct SpawnPoint
    {
        SpawnPoint (Aircraft::Type type, float x, float y) :
            type (type),
            x (x),
            y (y)
        {
        }

        Aircraft::Type type;
        float x;
        float y;
    };


private:
    sf::RenderTarget& m_target;
    sf::RenderTexture m_sceneTexture;

    sf::View m_worldView;
    sf::View m_viewTeam2;

    TextureHolder m_textures;
    FontHolder& m_fonts;
    SoundPlayer& m_sounds;

    SceneNode m_sceneGraph;
    //m_sceneLayers est un conteneur composé de sceneNode associé à un layer count.
    //Ici, il y a donc un sceneNode pour background, un autre pour Air, etc...
    std::array<SceneNode*, LayerCount> m_sceneLayers;
    CommandQueue m_commandQueue;

    sf::FloatRect m_worldBounds;
    ///<  Définit la Position de la vue initiale pour l'équipe 1.
    sf::Vector2f m_spawnPosition;

    ///<  Définit la Position de la vue initiale pour l'équipe 2.
    sf::Vector2f m_spawnPositionTeam2;

    float m_scrollSpeed;
    float m_scrollSpeedCompensation;
    std::vector<Aircraft*>  m_playerAircrafts;

    std::vector<SpawnPoint> m_enemySpawnPoints;
    std::vector<Aircraft*> m_activeEnemies;

    BloomEffect m_bloomEffect;

    bool m_networkedWorld;
    NetworkNode* m_networkNode;
    SpriteNode* m_finishSprite;

    ///> Vector contenant l'ensemble des rectangles composant la grille
    ///> de collision.
    std::vector<sf::FloatRect> m_grilleDeCollision;

    ///> Nombre de case de collision en Y
    sf::Int32 m_nbCutY;
    ///> Nombre de case de collision en X
    sf::Int32 m_nbCutX;

};

#endif // WORLD_H
