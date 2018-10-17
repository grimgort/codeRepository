#ifndef BUTTON_H
#define BUTTON_H

#include <Component.h>
#include <ResourceIdentifiers.h>
#include <State.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>

class SoundPlayer;
/*
Permet de fabriquer et gérer les boutons.
*/
namespace GUI
{
class Button : public Component
{
public:
    typedef std::shared_ptr<Button> Ptr;
    typedef std::function<void()> CallBack;

    enum Type
    {
        Normal,
        Selected,
        Pressed,
        ButtonCount
    };

public:
    Button(State::Context context);

    void setCallBack(CallBack callBack);
    void setText(const std::string& text);
    void setToggle(bool flag);

    virtual bool isSelectable() const;
    virtual void select();
    virtual void deselect();

    virtual void activate();
    virtual void deactivate();

    virtual void handleEvent(const sf::Event& event);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void changeTexture(Type buttonType);

private:
    CallBack m_callBack;
    sf::Sprite m_sprite;
    sf::Text m_text;
    bool m_isToggle;
    SoundPlayer& m_sounds;
};

}

#endif // BUTTON_H
