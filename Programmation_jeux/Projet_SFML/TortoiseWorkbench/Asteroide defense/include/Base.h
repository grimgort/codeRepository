#ifndef BASE_H
#define BASE_H

#include <Entity.h>
#include <Command.h>
#include <ResourceIdentifiers.h>
#include <Animation.h>
#include <TextNode.h>

#include <SFML/Graphics/Sprite.hpp>

/////////////////////////////////////////////////
/// \file Base.h
/// \brief Créer une base
/// \author Fred
/// \version 0.0
///
/// Les base hérite des Entity. C'est un objet immobile avec des PV.
///
/// Généralité : Pour Rajouter un objet qui hérites des entités,
/// Il faut compléter les fichier suivant : DataTable.h, DataTable.cpp,
/// RessourceIdentifier.h
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/// \class Base
/// \brief Classe representant les bases
///
/// La classe gère les bases. Se sont les objet immobile a défendre
/// pour le joueur ou à détruire pour l'équipe adverse.
/////////////////////////////////////////////////

class Base : public Entity
{
public:
    /////////////////////////////////////////////////
    /// \enum Définit le type de base
    ///
    /// Permet de définit les caractéristique de la base en fonction du type
    /// en allant récupérer les donnée dans la DataTables.
    /////////////////////////////////////////////////
    enum Type
    {
        BaseTypeUn,
        TypeCount
    };

public:
    /////////////////////////////////////////////////
    /// \brief Constructeur
    ///
    /// \param type : Type pouvant prendre la valeur BaseTypeUn ou TypeCount.
    /// Les type doivent être déclaré dans DataTable
    /// \param &textures : Référence de la texture de la base contenu
    /// dans TextureHolder.
    /// \param &fonts : Référence du type de font contenu
    /// dans FontHolder
    /////////////////////////////////////////////////
    Base (Type type, const TextureHolder &textures,
          const FontHolder &fonts);

    /////////////////////////////////////////////////
    /// \brief Retourne la catégorie de la base
    ///
    /// Retourne une catégorie Base. (Il faut certainement faire
    /// une catégorie base allié et ennemy... a corriger)
    /////////////////////////////////////////////////
    virtual unsigned int getCategory() const;

    /////////////////////////////////////////////////
    /// \brief Retourne la position global du sprite
    ///
    /// Retourne une catégorie Base. (Il faut certainement faire
    /// une catégorie base allié et ennemy... a corriger)
    /////////////////////////////////////////////////
    virtual sf::FloatRect getBoundingRect() const;

    /////////////////////////////////////////////////
    /////////////////////////////////////////////////
    /// \brief Détruit le noeud base dans la mémoire
    ///
    /// Aprés son utilisation, cet objet n'existe plus.
    /////////////////////////////////////////////////
    virtual void remove();

    /////////////////////////////////////////////////
    /// \brief Dit si l'objet doit être détruit à cet frame
    ///
    /// \return Vrai si l'objet est détruit et l'explosion terminé
    /////////////////////////////////////////////////
    virtual bool isMarkedForRemoval() const;

    /////////////////////////////////////////////////
    /// \brief Joue un son avec l'ID effect
    ///
    /// Insére un son dans la pile de commande.
    /// Utilisé pour l'explosion de la base
    /////////////////////////////////////////////////
    void playLocalSound (CommandQueue& commands, SoundEffect::ID effect);

    /////////////////////////////////////////////////
    /// \brief Placement dans la grille de collision de la base
    ///
    /// \param virtualRectCollision : tableau de la grille de collision
    /// \param collisionListeToTest : multimap contenant l'ensemble des entités avec leur position
    /// \param nbCutX : nombre de grille en X (horizontale)
    /// \param nbCutY : nombre de grille en Y (verticale)
    ///
    /// Remplit le multimap "collisionListeToTest" afin de
    /// tester les collisions entre les entités de la même case.
    ///
    /// \bug il a était nécéssaire de rajouter une fonction de collision
    /// pour la base car il est situé sur l'ensemble des case virtuel de
    /// collision horizontale. Ainsi, regarder sa grille de collsion autour
    /// de la case enregistré dans m_positionCollision est insuffisant.
    /// Ha voir si l'on garde comme ca mais à optimiser dans tous les cas.
    /////////////////////////////////////////////////
    virtual void checkNodePosition (const std::vector<sf::FloatRect>
                                    &virtualRectCollision
                                    , std::multimap<int, SceneNode*>
                                    &collisionListeToTest
                                    , sf::Int32 nbCutX
                                    , sf::Int32 nbCutY);

private:
    /////////////////////////////////////////////////
    /// \brief Trace la texture de la base
    ///
    /// Vérifie si l'objet est détruit ou pas. Dans le premier cas,
    /// l'explosion est montré. Dans le deuxième, la base est montré.
    /////////////////////////////////////////////////
    virtual void drawCurrent (sf::RenderTarget & target
                              , sf::RenderStates states) const;

    /////////////////////////////////////////////////
    /// \brief Met a jour la base pour la frame
    ///
    /// Si la base est détruite, le son de l'explosion est joué.
    /// On utilise aussi la fonction updateCurrent de l'entité.
    /////////////////////////////////////////////////
    virtual void updateCurrent (sf::Time dt, CommandQueue& commands);
    /////////////////////////////////////////////////
    /// \brief Met a jour le texte (point de vie de la base)
    ///
    /////////////////////////////////////////////////
    void updateTexts();


private:
    ///> Contient le type de la base (Base)
    Type m_type;

    ///> Contient le sprite de la base
    sf::Sprite m_sprite;

    ///> Vrai si l'animation explosion doit être montré
    bool m_showExplosion;

    ///> Vrai si l'animation explosion doit commencer
    bool m_explosionBegan;

    ///> Animation de l'explosion des bases
    Animation m_explosion;

    ///> Point de vie de la base
    TextNode* m_healthDisplay;

};

#endif // BASE_H
