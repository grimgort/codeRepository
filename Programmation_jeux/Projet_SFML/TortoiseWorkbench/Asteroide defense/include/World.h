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
///  La classe contient l'ensemble des textures, des entit�es, etc...
///  Lorsque l'on appui sur les bouton play on cr�er cette classe.
/////////////////////////////////////////////////
class World : private sf::NonCopyable
{
public:
    /////////////////////////////////////////////////
    /// \brief Constructeur
    ///
    /// \param outputTarget : Fen�tre de l'application
    /// \param fonts : Conteneur des fonts du texte
    /// \param sounds : Class qui joue et contient les sons
    /// \param networked : D�fini si le jeux est en ligne ou local
    ///
    /// Met en place le monde en chargant l'ensemble des texture de
    /// d�part. D�finie la position de la vue de d�part.
    /////////////////////////////////////////////////
    explicit World (sf::RenderTarget& outputTarget, FontHolder& fonts,
                    SoundPlayer& sounds, bool networked = false);

    /////////////////////////////////////////////////
    /// \brief Met � jour l'ensemble des noeuds du world
    ///
    /// \param dt : Pas de temps du jeux
    ///
    /// Permet d'ajuster la vue des joueurs (2max).
    /// D�truit les entit�es en dehors du monde.
    /// Guide les missiles.
    /// applique les actions des commandes.
    /// Lance le calcul des collisions.
    /// Met a jour les sons.
    /////////////////////////////////////////////////
    void update (sf::Time dt);

    /////////////////////////////////////////////////
    /// \brief Trace les vues et les textures associ�s
    ///
    /// Applique le bloom effect si la carte graphique le supporte.
    /////////////////////////////////////////////////
    void draw();

    /////////////////////////////////////////////////
    /// \brief R�cup�re le rectangle d�finissant la vue 1
    ///
    /// \return FloatRect de la vue 1
    /////////////////////////////////////////////////
    sf::FloatRect getViewBounds() const;

    /////////////////////////////////////////////////
    /// \brief R�cup�re le rectangle d�finissant la vue 2
    ///
    /// \return FloatRect de la vue 2
    /////////////////////////////////////////////////
    sf::FloatRect getViewBoundsTeam2() const;

    /////////////////////////////////////////////////
    /// \brief R�cup�re la liste des commandes en cours
    ///
    /// \return La r�f�rence de la liste des commandes en cours
    /////////////////////////////////////////////////
    CommandQueue& getCommandQueue();

    /////////////////////////////////////////////////
    /// \brief Ajoute un avion de type joueur
    ///
    /// \param identifier : identifiant de l'avion cr�er
    /// \return Pointeur de l'avion cr�er
    ///
    /// Cr�er un pointeur unique d'un avion joueur et d�fini sa position,
    /// son identifiant et son noeud p�re.
    /////////////////////////////////////////////////
    Aircraft* addAircraft (int identifier);

    /////////////////////////////////////////////////
    /// \brief D�truit un avion de type joueur reconnu par son identifiant
    ///
    /// \param identifier : identifiant de l'avion cr�er
    /////////////////////////////////////////////////
    void removeAircraft (int identifier);

    /////////////////////////////////////////////////
    /// \brief A compl�ter
    ///
    /// \param lineY : position en y
    ///
    /// Utilis� pour la partie network.
    /////////////////////////////////////////////////
    void setCurrentBattleFieldPosition (float lineY);

    /////////////////////////////////////////////////
    /// \brief D�finie la hauteur du niveau
    ///
    /// \param height : Hauteur choisit
    ///
    /// Utilis� pour la partie network.
    /////////////////////////////////////////////////
    void setWorldHeight (float height);

    /////////////////////////////////////////////////
    /// \brief Ajoute des avions ennemies
    ///
    /// \param type : type de l'avion a cr�er
    /// \param relX : spawn en X de l'avion
    /// \param relY : spawn en Y de l'avion
    ///
    /////////////////////////////////////////////////
    void addEnemy (Aircraft::Type type, float relX, float relY);

    /////////////////////////////////////////////////
    /// \brief Trie l'ordre des avions ennemies � cr�er
    ///
    /// obsolette dans la version actuel car les avions
    /// ennemies vont disparaitre
    /////////////////////////////////////////////////
    void sortEnemies();

    /////////////////////////////////////////////////
    /// \brief V�rifie que des avion joueurs sont encore pr�sent
    ///
    /// \return Vrai si il reste au moin un joueur vivant
    /////////////////////////////////////////////////
    bool hasAlivePlayer() const;

    /////////////////////////////////////////////////
    /// \brief V�rifie si un joueur a atteint la ligne d'arriv�
    ///
    /// \return vrai si la ligne d'arriv� est atteinte
    ///
    /// Obsolette car plus de ligne d'arriv�
    /////////////////////////////////////////////////
    bool hasPlayerReachedEnd() const;

    /////////////////////////////////////////////////
    /// \brief D�finit la scroll compensation (network)
    ///
    /// \param compensation : valeur de la compensation
    /////////////////////////////////////////////////
    void setWorldScrollCompensation (float compensation);

    /////////////////////////////////////////////////
    /// \brief R�cup�re le pointeur de l'avion joueur
    ///
    /// \param identifier : identifiant de l'avion joueur � r�cup�rer
    /// \return Pointeur de l'avion
    /////////////////////////////////////////////////
    Aircraft* getAircraft (int identifier) const;

    /////////////////////////////////////////////////
    /// \brief R�cup�re les bordures du monde
    ///
    /// \return : le rectangle des bordure de l'�cran.
    /////////////////////////////////////////////////
    sf::FloatRect getBattlefieldBounds() const;

    /////////////////////////////////////////////////
    /// \brief R�cup�re les dimension de la carte world
    ///
    /// \return Le rectangle du monde
    /////////////////////////////////////////////////
    sf::FloatRect getWorldBounds() const;

    /////////////////////////////////////////////////
    /// \brief Cr�er un drop de bonus (pickup)
    ///
    /// \param position : Vecteur d�finissant la position du drop
    /// \param type : Type du pickup (vie, d�gat, etc...)
    /////////////////////////////////////////////////
    void createPickup (sf::Vector2f position, Pickup::Type type);

    /////////////////////////////////////////////////
    /// \brief A d�finir (network)
    ///
    /// \param out : ??
    /////////////////////////////////////////////////
    bool pollGameAction (GameActions::Action& out);

private:
    /////////////////////////////////////////////////
    /// \brief Charge les textures � afficher dans le monde
    ///
    /// Les nouvelles textures doivent �tre ajouter ici
    /////////////////////////////////////////////////
    void loadTextures();

    /////////////////////////////////////////////////
    /// \brief Adapte la position de l'avion en fonction des bordures du monde
    ///
    /// Evite que l'avion joueur d�passe les bordures du jeux.
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
    /// de coupe en X(m_nbCutX) et Y(m_nbCutY) d�finit par le d�velloper.
    /// La m�thode compl�te donc le vecteur m_grilleDeCollision qui
    /// contient l'ensemble des rectangle de la grille de collision.
    ///
    /////////////////////////////////////////////////
    void grilleDeCollision();

    /////////////////////////////////////////////////
    /// \brief D�clenche le calcul des collision
    ///
    /// R�cup�re la grille de collision et demande la position des
    /// noeud dans la grille, regarde si les noeud rentre en collision
    /// et applique l'effet des collision.
    ///
    /// Id�e : Il est peut �ter int�rr�ssant d'en faire une classe (h�ritant
    /// du world???)
    ///
    /////////////////////////////////////////////////
    void handleCollisions();

    /////////////////////////////////////////////////
    /// \brief Met � joueur le son jou� en fonction de la position du joueur
    ///
    /// Met a jour le son jou� en fonction de la position de l'avion joueur
    /////////////////////////////////////////////////
    void updateSounds();

    /////////////////////////////////////////////////
    /// \brief Construit les objet de la scene � l'�tat initial
    ///
    /// Permet de construire l'ensemble des objet de la scene � l'�tat
    /// initial. C'est donc ici qu'il faut rajouter les objets qui doivent
    /// �tre construit au d�but de la scene. Elle est donc invoquer
    /// dans le constructeur de la classe World.
    /////////////////////////////////////////////////
    void buildScene();

    /////////////////////////////////////////////////
    /// \brief Ajoute la totalit� des avions ennemies
    ///
    /// Les avions ennemie � cr�er sont a rajouter ici
    /////////////////////////////////////////////////
    void addEnemies();

    /////////////////////////////////////////////////
    /// \brief Ajoute les noeuds ennemies � la scene
    ///
    /// fonction secondaire � addEnemies.
    /////////////////////////////////////////////////
    void spawnEnemies();

    /////////////////////////////////////////////////
    /// \brief D�truit informatiquement les entit� qui sont en-dehors du monde
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
    //m_sceneLayers est un conteneur compos� de sceneNode associ� � un layer count.
    //Ici, il y a donc un sceneNode pour background, un autre pour Air, etc...
    std::array<SceneNode*, LayerCount> m_sceneLayers;
    CommandQueue m_commandQueue;

    sf::FloatRect m_worldBounds;
    ///<  D�finit la Position de la vue initiale pour l'�quipe 1.
    sf::Vector2f m_spawnPosition;

    ///<  D�finit la Position de la vue initiale pour l'�quipe 2.
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
