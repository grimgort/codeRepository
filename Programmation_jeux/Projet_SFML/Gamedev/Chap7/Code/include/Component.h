#ifndef COMPONENT_H
#define COMPONENT_H

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>

namespace sf
{
class Event;
}

namespace GUI
{

class Component : public sf::Transformable, public sf::NonCopyable, public sf::Drawable
{
public:
    typedef std::shared_ptr<Component> Ptr;

public:
    Component();
    virtual ~Component();

    virtual bool isSelectable() const = 0;
    bool isSelected();
    virtual void select();
    virtual void deselect();

    virtual bool isActive() const;
    virtual void activate();
    virtual void deactivate();

    virtual void handleEvent(const sf::Event& event) = 0;

private:
    bool m_isSelected;
    bool m_isActive;
};

}
#endif // COMPONENT_H
