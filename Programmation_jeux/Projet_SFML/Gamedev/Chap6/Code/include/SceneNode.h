#ifndef SCENENODE_H
#define SCENENODE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <map>
#include <memory>
#include <assert.h>
#include <Command.h>

/*
Permet de définir les noeuds de la scéne (position des objets dans la fenêtre).
Chaque noeud est unique(nonCopyable), peuvent être dessiner(Drawable) et modifier(Transformable).
Les méthodes sont écrites en considérant le noeud manipulé comme le noeud parent
*/
class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
    typedef std::unique_ptr<SceneNode> Ptr;

    SceneNode();
    void attachChild(Ptr child);
    Ptr detacheChild(const SceneNode& node);
    void update(sf::Time dt);
    sf::Vector2f getWorldPosition() const;
    sf::Transform getWorldTransform() const;

    void onCommand(const Command& command, sf::Time dt);
	virtual unsigned int getCategory() const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states  ) const;
    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
    void drawChildren( sf::RenderTarget& target, sf::RenderStates states ) const;
    virtual void updateCurrent(sf::Time dt);
    void updateChildren(sf::Time dt);

private:
    std::vector<Ptr> m_children; //tableau contenant les pointeurs des noeuds des enfants.
    SceneNode* m_parent; // récupére l'adresse du noeud du père.
};

#endif // SCENENODE_H
