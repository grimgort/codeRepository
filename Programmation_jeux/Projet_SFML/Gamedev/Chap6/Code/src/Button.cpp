#include <Button.h>
#include <Utility.h>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{

Button::Button(const FontHolder& fonts, const TextureHolder& textures) :
    m_callBack(),
    m_normalTexture(textures.get(Textures::ButtonNormal)),
    m_selectedTexture(textures.get(Textures::ButtonSelected)),
    m_pressedTexture(textures.get(Textures::ButtonPressed)),
    m_sprite(),
    m_text("",fonts.get(Fonts::Main), 16),
    m_isToggle(false)
{
    //applique la texture du bouton normal aux boutton
    m_sprite.setTexture(m_normalTexture);
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
    m_sprite.setTexture(m_selectedTexture);
}

/*
désélectionne le bouton
*/
void Button::deselect()
{
    Component::deselect();
    m_sprite.setTexture(m_normalTexture);
}

/*
Active le bouton
*/
void Button::activate()
{
    //le component bouton devient actif avec m_isActive = true
    Component::activate();

    //
    if (m_isToggle) m_sprite.setTexture(m_pressedTexture);
    if (m_callBack) m_callBack();
    //
    if (!m_isToggle) deactivate();
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
        if (isSelectable()) m_sprite.setTexture(m_selectedTexture);
        else (m_sprite.setTexture(m_normalTexture));
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

}
