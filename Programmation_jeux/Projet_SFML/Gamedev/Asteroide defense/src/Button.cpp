#include <Button.h>
#include <Utility.h>
#include <SoundPlayer.h>
#include <ResourceHolder.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{

Button::Button(State::Context context)
    : m_callBack()
    , m_sprite(context.textures->get(Textures::Buttons))
    , m_text("",context.fonts->get(Fonts::Main), 16)
    , m_isToggle(false)
    , m_sounds(*context.sounds)
{
    //applique la texture du bouton normal aux boutton
    changeTexture(Normal);
    //récupére les coordonnées du boutton
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    //définit la position du texte du boutton
    m_text.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}

void Button::setCallBack(CallBack callBack)
{
    m_callBack = std::move(callBack);
}

/*
Applique le text au bouton
*/
void Button::setText(const std::string& text)
{
    m_text.setString(text);
    centerOrigin(m_text);
}

/*

*/
void Button::setToggle(bool flag)
{
    m_isToggle = flag;
}

/*
fonction pour dire que le bouton est séléctionable
*/
bool Button::isSelectable() const
{
    return true;
}

/*
Selectionne le bouton
*/
void Button::select()
{
    //définit le component bouton comme séléctionné car m_isSelected devient true
    Component::select();
    //applique la texture de séléction au bouton
    changeTexture(Selected);
}

/*
désélectionne le bouton
*/
void Button::deselect()
{
    Component::deselect();
    changeTexture(Normal);
}

/*
Active le bouton
*/
void Button::activate()
{
    //le component bouton devient actif avec m_isActive = true
    Component::activate();

    //
    if (m_isToggle)changeTexture(Pressed);
    if (m_callBack) m_callBack();
    //
    if (!m_isToggle) deactivate();

    m_sounds.play(SoundEffect::Button);
}

/*
Desactive le bouton
*/
void Button::deactivate()
{
    //m_isActive devient false
    Component::deactivate();
    if (m_isToggle)
    {
        if (isSelected())
            changeTexture(Selected);
        else
            changeTexture(Normal);
    }
}

void Button::handleEvent(const sf::Event&)
{
}

/*
Dessine le bouton et son texte
*/
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_sprite, states);
    target.draw(m_text, states);
}

void Button::changeTexture(Type buttonType)
{
    sf::IntRect textureRect(0, 50*buttonType, 200, 50);
    m_sprite.setTextureRect(textureRect);
}
}
