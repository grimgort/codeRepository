#ifndef SCENENODE_H
#define SCENENODE_H

#include <Category.h>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <set>
#include <memory>
#include <utility>
#include <map>

/////////////////////////////////////////////////
/// \file SceneNode.h
/// \brief Créer un noeud de type scene
/// \author Fred
/// \version 0.0
///
/////////////////////////////////////////////////

struct Command;
class CommandQueue;

/*
    Permet de définir les noeuds de la scéne (position des objets dans la fenêtre).
    Chaque noeud est unique(nonCopyable), peuvent être dessiner(Drawable) et modifier(Transformable).
    Les méthodes sont écrites en considérant le noeud manipulé comme le noeud parent
*/
/////////////////////////////////////////////////
/// \class SceneNode
/// \brief Classe representant le sceneNode
///
/// La classe gère les noeuds représantant l'ensemble des objets du jeux.
/// Par exemple, les avions, les projectiles, les pickup sont hérite des sceneNode
/// Les méthodes de cette classe permettent notamment de gérer la création et
/// la destruction des objets (ou noeud).
///
/// L'architecture des noeuds est un arbre. C'est à dire qu'il y a un noeud
/// principal qui posséde plusieurs branche avec d'autres noeud, etc...
///
/////////////////////////////////////////////////
//        *---*---*
//        |
// *------*   *---*
//        |   |
//        *---*---*
/////////////////////////////////////////////////
class SceneNode
    : public sf::Transformable
    , public sf::Drawable
    , private sf::NonCopyable
{
public:
    ///>Définit Ptr comme un pointeur unique de SceneNode
    typedef std::unique_ptr<SceneNode> Ptr;
    ///>Définit Pair comme une paire d'adresse de pointeur de SceneNode
    typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
    /////////////////////////////////////////////////
    /// \brief Constructeur
    ///
    /// \param category : Type de catégorie (Aircraft, missile, etc...).
    /// De base, c'est une category de type None
    /// (qui n'est pas encore définit par héritage)
    /////////////////////////////////////////////////
    explicit SceneNode (Category::Type category =
                            Category::None);

    /////////////////////////////////////////////////
    /// \brief Rajoute un noeud enfant
    ///
    /// \param child : pointeur de l'enfant
    ///
    /// Permet de rajouter un noeud enfant à se sceneNode en completant
    /// le vecteur "m_children" avec le pointeur.
    /////////////////////////////////////////////////
    void attachChild (Ptr child);

    /////////////////////////////////////////////////
    /// \brief Détache un noeud de son père
    ///
    /// \param node : pointeur de l'enfant
    /// \return Renvoit le pointeur (objet) afin de le déplacer
    /// quelque part
    ///
    /// Permet de récupérer un pointeur (et ses enfants) en le détachant du noeud prére
    /////////////////////////////////////////////////
    Ptr detacheChild (const SceneNode& node);

    /////////////////////////////////////////////////
    /// \brief met à jour le noeud
    ///
    /// \param dt : pointeur de l'enfant
    /// \param commands : Comande dans la queue
    ///
    /////////////////////////////////////////////////
    void update (sf::Time dt, CommandQueue& commands);

    /////////////////////////////////////////////////
    /// \brief Retourne la position absolu du noeud dans le niveau
    ///
    /// \return Le vecteur 2D de la position
    ///
    /// Se base sur getWorldTransform qui récupére
    /// le "transform" de la scéne mère.
    /////////////////////////////////////////////////
    sf::Vector2f getWorldPosition() const;

    /////////////////////////////////////////////////
    /// \brief Retourne le transform du noeud mère (niveau)
    ///
    /// \return Retourne le transform du noeud mère (niveau)
    ///
    /////////////////////////////////////////////////
    sf::Transform getWorldTransform() const;

    /////////////////////////////////////////////////
    /// \brief Applique les commandes sur le noeud
    ///
    ///
    ///
    /////////////////////////////////////////////////
    void onCommand (const Command& command
                    , sf::Time dt);

    /////////////////////////////////////////////////
    /// \brief Retourne la catégorie du noeud (m_nodeCategory)
    ///
    /// \return renvoit un entier correspondant à une catégorie comme
    /// définit dans Category.h
    ///
    /////////////////////////////////////////////////
    virtual unsigned int getCategory() const;

    /////////////////////////////////////////////////
    /// \brief Placement dans la grille de collision du noeud
    ///
    /// \param virtualRectCollision : tableau de la grille de collision
    /// \param collisionListeToTest : multimap contenant l'ensemble des entités avec leur position
    /// \param nbCutX : nombre de grille en X (horizontale)
    /// \param nbCutY : nombre de grille en Y (verticale)
    ///
    /// Fonction virtuel ne faisant rien pour un sceneNode mais place
    /// un noeud de type entitée dans la grille de collision
    ///
    /////////////////////////////////////////////////
    virtual void checkNodePosition (const std::vector<sf::FloatRect>
                                    &virtualRectCollision
                                    , std::multimap<int, SceneNode*>
                                    &collisionListeToTest
                                    , sf::Int32 nbCutX
                                    , sf::Int32 nbCutY);

    /////////////////////////////////////////////////
    /// \brief Regarde si il y a collision entre les objet de la collisionListeToTest
    ///
    /// \param collisionListeToTest : multimap contenant l'ensemble des entités avec
    /// leur position dans la grille de collision
    /// \param collisionPairs : Tableau contenant les noeuds
    /// en collision
    ///
    /// Remplit le tableau de collisionPairs avec les objets
    /// en collision. Il n'y a plus qu'à appliquer les
    /// conséquence des collisions sur les noeuds.
    ///
    /////////////////////////////////////////////////
    void checkSceneCollision (
        std::multimap<int, SceneNode*>
        &collisionListeToTest
        , std::set<Pair> &collisionPairs
        , sf::Int32 nbCutX
        , sf::Int32 nbCutY);

    /////////////////////////////////////////////////
    /// \brief Supprime les noeuds marqué en isMarkedForRemoval
    ///
    /// Détruit également tous les noeud enfant et l'ensemble de
    /// la généalogie.
    ///
    /////////////////////////////////////////////////
    void removeWrecks();

    /////////////////////////////////////////////////
    /// \brief Retourne le rectangle du noeud
    ///
    /////////////////////////////////////////////////
    virtual sf::FloatRect getBoundingRect() const;

    //    sf::Vector2f getWorldBounding() const;
    /////////////////////////////////////////////////
    /// \brief Demande si le noeud doit être supprimé
    ///
    /// \return Retourne True si le noeud doit être supprimé sinon False
    ///
    ///Invoque la fonction is destroyed qui retourne soit True soit False
    /// Un sceneNode basic n'est jamais marqué en removal
    /////////////////////////////////////////////////
    virtual bool isMarkedForRemoval() const;

    /////////////////////////////////////////////////
    /// \brief Demande si le noeud est détruit
    ///
    /// \return Retourne True si le noeud est détruit sinon False
    ///
    /// Un sceneNode basic n'est jamais détruit (pas de collision avec)
    /////////////////////////////////////////////////
    virtual bool isDestroyed() const;

    /////////////////////////////////////////////////
    /// \brief Retourne le numéro de la grille de collision
    ///
    /// \return -9999 si le noeud n'est pas concerné par les collisions
    /// sinon il renvoit un entier compris entre 1 et le nombre de grille
    /// de collision
    ///
    /// Un sceneNode basic renvoit toujours -9999 (pas de collision avec)
    /////////////////////////////////////////////////
    virtual int getPositionCollision() const;

private:

    /////////////////////////////////////////////////
    /// \brief Met à jour le noeud courant
    ///
    /// \param dt : Pas de temps
    /// \param &commands : Référence de la commande
    ///
    /// Ne fait rien dans le cas d'un sceneNode basic
    /////////////////////////////////////////////////
    virtual void updateCurrent (sf::Time dt
                                , CommandQueue &commands);
    /////////////////////////////////////////////////
    /// \brief Met à jour les noeuds enfant
    ///
    /// \param dt : Pas de temps
    /// \param &commands : Référence de la commande
    ///
    /////////////////////////////////////////////////
    void updateChildren (sf::Time dt
                         , CommandQueue &commands);

    /////////////////////////////////////////////////
    /// \brief Dessine le sprite du noeud
    ///
    /// \param &target : Référence de la cible
    /// \param &states : Référence de l'état
    ///
    /// Appel les fonctions  drawCurrent et  drawChildren
    /////////////////////////////////////////////////
    virtual void draw (sf::RenderTarget &target
                       , sf::RenderStates states) const;

    /////////////////////////////////////////////////
    /// \brief Dessine le noeud courant
    ///
    /// \param &target : Référence de la cible
    /// \param &states : Référence de l'état
    ///
    /////////////////////////////////////////////////
    virtual void drawCurrent (sf::RenderTarget &target
                              , sf::RenderStates states) const;

    /////////////////////////////////////////////////
    /// \brief Dessine les noeuds enfant
    ///
    /// \param &target : Référence de la cible
    /// \param &states : Référence de l'état
    ///
    /////////////////////////////////////////////////
    void drawChildren (sf::RenderTarget& target
                       , sf::RenderStates states) const;

    /////////////////////////////////////////////////
    /// \brief Dessine un rectangele (inutile actuellement)
    ///
    /// \param &target : Référence de la cible
    /// \param states : Référence de l'état
    ///
    /////////////////////////////////////////////////
    void drawBoundingRect (sf::RenderTarget& target
                           , sf::RenderStates states) const;

    /////////////////////////////////////////////////
    /// \brief Regarde si les catégorie des noeuds matche
    ///
    /// \param &node1 : Référence du noeud 1
    /// \param &node2 : Référence du noeud 2
    /// \param type1 : Catégorie 1 à comparé
    /// \param type2 : Catégorie 2 à comparé
    ///
    /// \return Retourne True si les catégories des noeuds correspondent
    /////////////////////////////////////////////////
    bool matchesCategories (SceneNode &node1
                            , SceneNode &node2
                            , Category::Type type1
                            , Category::Type type2);

private:
    ///> Tableau contenant les pointeurs des noeuds enfants.
    std::vector<Ptr> m_children;
    ///> Adresse du noeud père.
    SceneNode* m_parent;
    ///> Catégorie du noeud
    Category::Type m_nodeCategory;
};

/////////////////////////////////////////////////
/// \brief Etudie la collision entre deux rectangles
///
/// \param &lhs : Référence du noeud 1
/// \param &rhs : Référence du noeud 2
///
/// \return Retourne True si les rectangles associés au
/// deux noeuds sont en intersection
/////////////////////////////////////////////////
bool collision (const SceneNode &lhs,
                const SceneNode &rhs);

/////////////////////////////////////////////////
/// \brief Etudie la distance entre deux noeuds
///
/// \param &lhs : Référence du noeud 1
/// \param &rhs : Référence du noeud 2
///
/// \return Retourne la distance entre deux noeuds
///
/// Utilise la méthode getWorldPosition pour avoir la position
/// de chaque noeud
/////////////////////////////////////////////////
float distance (const SceneNode &lhs,
                const SceneNode &rhs);


#endif // SCENENODE_H
