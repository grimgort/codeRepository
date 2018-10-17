#include <Aircraft.h>
#include <ResourceHolder.hpp>
#include <Category.h>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


/*
Permet d'accéder à l'ID de la texture de l'avion grâce aux mots Eagle ou Raptor
*/
Textures::ID toTextureID(Aircraft::Type type)
{
	switch (type)
	{
		case Aircraft::Eagle:
			return Textures::Eagle;

		case Aircraft::Raptor:
			return Textures::Raptor;
	}

    return Textures::Eagle;
}

/*
type = namespace contenant les noms des textures pouvant être associés aux Aircraft.
m_type = attribut de type
textures = Passage en référence de la class TextureHolder contenant l'ensemble des textures du jeux.
m_sprite = récupére le sprite de la texture du TextureHolder définit par "type"(Eagle ou Raptor).
            "textures.get(toTextureID(type))" récupére le pointeur de la class texture associé à l'ID "type".
            La déclaration "sf::Sprite" de m_sprite associe m_sprite à la sous-classe de Texture nommé Sprite.
*/
Aircraft::Aircraft(Type type,const TextureHolder& textures) : m_type(type), m_sprite(textures.get(toTextureID(type)))
{
    sf::FloatRect bounds = m_sprite.getLocalBounds(); // méthode de Sprite récupérant la position de l'image.
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f); // méthode de sprite définissant la position de l'image.
}

/*
Permet de dessiner les textures d'aircraft sur l'écran.

"RenderTarget" est une class de SFML qui permet de donner la fenêtre où doit être dessiner la figure.
"RenderStates" est une class de SFML qui permet de donner les transformations du sprite (rotaton; shader, etc...)
*/
void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite,states);
}

/*
Permet d'accéder à la catégory du Aircraft en fonction de l'ID de la texture(Eagle ou Raptor).
Cela renvoit un entier différent en fonction de la catégorie, qui est définit dans Catégory.h
*/
unsigned int Aircraft::getCategory() const
{
    switch (m_type)
    {
    case Eagle:
        return Category::PlayerAircraft;

    default:
        return Category::EnemyAircraft;
    }
}


