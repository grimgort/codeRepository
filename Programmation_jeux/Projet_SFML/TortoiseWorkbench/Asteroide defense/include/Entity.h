#ifndef ENTITY_H
#define ENTITY_H

#include <SceneNode.h>
#include <map>
/////////////////////////////////////////////////
/// \file Entity.h
/// \brief Créer une entité
/// \author Fred
/// \version 0.0
///
/// Les entités hérite des sceneNode. Se sont des objets du jeux qui ont des PV/Collision/Vitesse
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/// \class Entity
/// \brief Classe representant l'entité
///
///  La classe gère les entités
/////////////////////////////////////////////////
class Entity : public SceneNode
{
public:
    /////////////////////////////////////////////////
    /// \brief Constructeur
    ///
    /// \param hitpoints : Nombre de point de vie de l'entité
    /////////////////////////////////////////////////
    explicit Entity (int hitpoints);
    /////////////////////////////////////////////////
    /// \brief Donne une vitesse à l'entité(vecteur)
    ///
    /// \param velocity : vecteur 2D définissant la vitesse
    ///
    /// Permet de donner une vitesse à une entité à partir d'un vecteur
    /////////////////////////////////////////////////
    void setVelocity (sf::Vector2f velocity);
    /////////////////////////////////////////////////
    /// \brief Donne une vitesse à l'entité(composante)
    ///
    /// \param vx : vitesse en x (horizontale)
    /// \param vy : vitesse en y (vertical)
    ///
    /// Permet de donner une vitesse à une entité à partir des composante en x et y.
    ///
    /////////////////////////////////////////////////
    void setVelocity (float vx
                      , float vy);
    /////////////////////////////////////////////////
    /// \brief Donne une accélération à l'entité(vecteur)
    ///
    /// \param velocity : vecteur 2D définissant l'accélération.
    ///
    /// Permet de donner une accélération à une entité à partir d'un vecteur
    /////////////////////////////////////////////////
    void accelerate (sf::Vector2f velocity);
    /////////////////////////////////////////////////
    /// \brief Donne une accélération à l'entité(composante)
    ///
    /// \param velocity : vecteur 2D définissant l'accélération.
    ///
    /// Permet de donner une accélération à une entité à partir des composante en x et y.
    /////////////////////////////////////////////////
    void accelerate (float vx
                     , float vy);
    /////////////////////////////////////////////////
    /// \brief Retourne la vitesse de l'entité
    ///
    /// \return le vecteur de la vitesse
    ///
    /////////////////////////////////////////////////
    sf::Vector2f getVelocity() const;
    /////////////////////////////////////////////////
    /// \brief Retourne les points de vie de l'entité
    ///
    /// \return les points de vie
    ///
    /////////////////////////////////////////////////
    int getHitpoints() const;
    /////////////////////////////////////////////////
    /// \brief Définit les points de vie de l'entité
    ///
    /// \param points : point de vie
    ///
    /////////////////////////////////////////////////
    void setHitpoints (int points);
    /////////////////////////////////////////////////
    /// \brief Répare l'entité
    ///
    /// \param points : point de vie
    ///
    /// Permet de réparer une entité en lui rajoutant "points" points de vie
    /////////////////////////////////////////////////
    void repair (int points);
    /////////////////////////////////////////////////
    /// \brief Endomage l'entité
    ///
    /// \param points : point de vie
    ///
    /// Permet d'infliger X point de degat à l'entité en enlevant "points" points de vie
    /////////////////////////////////////////////////
    void damage (int points);
    /////////////////////////////////////////////////
    /// \brief Détruit l'entité
    ///
    /// Met les points de vie de l'entité à 0.
    /////////////////////////////////////////////////
    void destroy();
    /////////////////////////////////////////////////
    /// \brief Détruit l'entité
    ///
    /// Appel la fonction destroy qui met les point de vie de l'entité à 0.
    /// C'est une fonction virtuelle contrairement à destroy!
    /////////////////////////////////////////////////
    virtual void remove();
    /////////////////////////////////////////////////
    /// \brief Demande si l'entité est détruite
    ///
    /// \return True si les point de vie de l'entité sont inférieur ou égal à 0 sinon false
    ///
    /////////////////////////////////////////////////
    virtual bool isDestroyed() const;
    /////////////////////////////////////////////////
    /// \brief Placement dans la grille de collision de l'entité
    ///
    /// \param virtualRectCollision : tableau de la grille de collision
    /// \param collisionListeToTest : multimap contenant l'ensemble des entités avec leur position
    /// \param nbCutX : nombre de grille en X (horizontale)
    /// \param nbCutY : nombre de grille en Y (verticale)
    ///
    /// Remplit le multimap "collisionListeToTest" afin de
    /// tester les collisions entre les entités de la même case.
    ///
    /// Attention : il est possible que l'on test des
    /// virtualRectCollision[i<0]. Le résultat retourné est
    /// définit comme incertain. A éventuellement corriger
    ///
    /////////////////////////////////////////////////
    virtual void checkNodePosition (const std::vector<sf::FloatRect>
                                    &virtualRectCollision
                                    , std::multimap<int, SceneNode*>
                                    &collisionListeToTest
                                    , sf::Int32 nbCutX
                                    , sf::Int32 nbCutY);
    /////////////////////////////////////////////////
    /// \brief Retourne l'emplacement dans la grille de collision
    ///
    /// \return numéro de grille ou -9999 si ce n'est pas une entité
    ///
    /////////////////////////////////////////////////
    virtual int getPositionCollision() const ;

protected:
    /////////////////////////////////////////////////
    /// \brief Met à jour la nouvelle position de l'entité
    ///
    /// \param dt : Pas de temps de la frame
    /// \param commands : la queue des commandes
    ///
    /// Multiplie la vitesse par le pas de temps et déplace
    /// l'entité du résultat de l'équation
    /////////////////////////////////////////////////
    virtual void updateCurrent (sf::Time dt
                                , CommandQueue &commands);

private:
    ///<  Vecteur 2D de la vitesse de l'entité.
    sf::Vector2f m_velocity;

    ///< Point de vie de l'entité.
    int m_hitPoints;

    ///< Numéro de rectangle de la grille de collision.
    int m_positionCollision;
};

#endif // ENTITY_H
