#ifndef RESOURCEIDENTIFIERS_H_INCLUDED
#define RESOURCEIDENTIFIERS_H_INCLUDED


// Conteneur "sf" est le conteneur définissant les classe de SFML
namespace sf
{
	class Texture;
	class Font;
	class Shader;
}

namespace Textures
{
    enum ID
    {
        Entities,
        Jungle,
        TitleScreen,
        Buttons,
        Explosion,
        Particle,
        FinishLine,
    };
}

namespace Shaders
{
    enum ID
    {
        BrightnessPass,
        DownSamplePass,
        GaussianBlurPass,
        AddPass,
    };
}

// Pareil que pour "Textures" mais avec la classe Font de SFML
namespace Fonts
{
	enum ID
	{
		Main,
	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;
/*
ResourceHolder<sf::Texture, Textures::ID> TextureHolder = Template de classe ResourceHolder définit
en tant que conteneur des classes Texture de SFML et nommé comme classe RessourceHolder.
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder = Pareil mais pour la classe Font de SFML
*/
typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder;
typedef ResourceHolder<sf::Shader, Shaders::ID> ShaderHolder;

#endif // RESOURCEIDENTIFIERS_H_INCLUDED
