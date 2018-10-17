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
/// \brief Cr�er une base
/// \author Fred
/// \version 0.0
///
/// Les base h�rite des Entity. C'est un objet immobile avec des PV.
///
/// G�n�ralit� : Pour Rajouter un objet qui h�rites des entit�s,
/// Il faut compl�ter les fichier suivant : DataTable.h, DataTable.cpp,
/// RessourceIdentifier.h
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/// \class Base
/// \brief Classe representant les bases
///
/// La classe g�re les bases. Se sont les objet immobile a d�fendre
/// pour le joueur ou � d�truire pour l'�quipe adverse.
/////////////////////////////////////////////////

class Base : public Entity
{
public:
    /////////////////////////////////////////////////
    /// \enum D�finit le type de base
    ///
    /// Permet de d�finit les caract�ristique de la base en fonction du type
    /// en allant r�cup�rer les donn�e dans la DataTables.
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
    /// Les type doivent �tre d�clar� dans DataTable
    /// \param &textures : R�f�rence de la texture de la base contenu
    /// dans TextureHolder.
    /// \param &fonts : R�f�rence du type de font contenu
    /// dans FontHolder
    /////////////////////////////////////////////////
    Base (Type type, const TextureHolder &textures,
          const FontHolder &fonts);

    /////////////////////////////////////////////////
    /// \brief Retourne la cat�gorie de la base
    ///
    /// Retourne une cat�gorie Base. (Il faut certainement faire
    /// une cat�gorie base alli� et ennemy... a corriger)
    /////////////////////////////////////////////////
    virtual unsigned int getCategory() const;

    /////////////////////////////////////////////////
    /// \brief Retourne la position global du sprite
    ///
    /// Retourne une cat�gorie Base. (Il faut certainement faire
    /// une cat�gorie base alli� et ennemy... a corriger)
    /////////////////////////////////////////////////
    virtual sf::FloatRect getBoundingRect() const;

    /////////////////////////////////////////////////
    /////////////////////////////////////////////////
    /// \brief D�truit le noeud base dans la m�moire
    ///
    /// Apr�s son utilisation, cet objet n'existe plus.
    /////////////////////////////////////////////////
    virtual void remove();

    /////////////////////////////////////////////////
    /// \brief Dit si l'objet doit �tre d�truit � cet frame
    ///
    /// \return Vrai si l'objet est d�truit et l'explosion termin�
    /////////////////////////////////////////////////
    virtual bool isMarkedForRemoval() const;

    /////////////////////////////////////////////////
    /// \brief Joue un son avec l'ID effect
    ///
    /// Ins�re un son dans la pile de commande.
    /// Utilis� pour l'explosion de la base
    /////////////////////////////////////////////////
    void playLocalSound (CommandQueue& commands, SoundEffect::ID effect);

    /////////////////////////////////////////////////
    /// \brief Placement dans la grille de collision de la base
    ///
    /// \param virtualRectCollision : tableau de la grille de collision
    /// \param collisionListeToTest : multimap contenant l'ensemble des entit�s avec leur position
    /// \param nbCutX : nombre de grille en X (horizontale)
    /// \param nbCutY : nombre de grille en Y (verticale)
    ///
    /// Remplit le multimap "collisionListeToTest" afin de
    /// tester les collisions entre les entit�s de la m�me case.
    ///
    /// \bug il a �tait n�c�ssaire de rajouter une fonction de collision
    /// pour la base car il est situ� sur l'ensemble des case virtuel de
    /// collision horizontale. Ainsi, regarder sa grille de collsion autour
    /// de la case enregistr� dans m_positionCollision est insuffisant.
    /// Ha voir si l'on garde comme ca mais � optimiser dans tous les cas.
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
    /// V�rifie si l'objet est d�truit ou pas. Dans le premier cas,
    /// l'explosion est montr�. Dans le deuxi�me, la base est montr�.
    /////////////////////////////////////////////////
    virtual void drawCurrent (sf::RenderTarget & target
                              , sf::RenderStates states) const;

    /////////////////////////////////////////////////
    /// \brief Met a jour la base pour la frame
    ///
    /// Si la base est d�truite, le son de l'explosion est jou�.
    /// On utilise aussi la fonction updateCurrent de l'entit�.
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

    ///> Vrai si l'animation explosion doit �tre montr�
    bool m_showExplosion;

    ///> Vrai si l'animation explosion doit commencer
    bool m_explosionBegan;

    ///> Animation de l'explosion des bases
    Animation m_explosion;

    ///> Point de vie de la base
    TextNode* m_healthDisplay;

};

#endif // BASE_H
