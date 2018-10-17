#include "TextNode.h"
#include <Utility.h>

#include <SFML/Graphics/RenderTarget.hpp>

TextNode::TextNode (const FontHolder& fonts, const std::string& text)
{
    m_text.setFont (fonts.get (Fonts::Main));
    m_text.setCharacterSize (20);
    setString (text);
}

void TextNode::drawCurrent (sf::RenderTarget& target,
                            sf::RenderStates states) const
{
    target.draw (m_text, states);
}

void TextNode::setString (const std::string& text)
{
    m_text.setString (text);
    centerOrigin (m_text);
}





