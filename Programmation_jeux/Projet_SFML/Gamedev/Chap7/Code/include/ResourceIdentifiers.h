#ifndef RESOURCEIDENTIFIERS_H_INCLUDED
#define RESOURCEIDENTIFIERS_H_INCLUDED


// Conteneur "sf" est le conteneur définissant les classe de SFML
namespace sf
{
	class Texture;
	class Font;
}

/*
Conteneur "Textures" contenant un tableau enum nommé "ID" contenant l'ensemble des identifiants des textures
manipuler par la class Texture de SFML. Ici, il y a donc 4 class texture de définit (Eagle, Raptor, Desert, TitleScreen)
*/
namespace Textures
{
	enum ID
	{
		Eagle,
		Raptor,
		Avenger,
		Bullet,
		Missile,
		Desert,
		HealthRefill,
		MissileRefill,
		FireSpread,
		FireRate,
		TitleScreen,
        ButtonNormal,
		ButtonSelected,
		ButtonPressed
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

#endif // RESOURCEIDENTIFIERS_H_INCLUDED
