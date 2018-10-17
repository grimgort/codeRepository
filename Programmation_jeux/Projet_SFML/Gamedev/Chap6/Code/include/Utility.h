#ifndef UTILITY_H
#define UTILITY_H

#include <SFML/Window/Keyboard.hpp>

#include <sstream>

namespace sf
{
    class Sprite;
    class Text;
}

template <typename T>
std::string toString(const T& value);


// Convert enumerators to strings
std::string toString(sf::Keyboard::Key key);

// Call setOrigin() with the center of the object
void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

#include <Utility.inl>

#endif // UTILITY_H
