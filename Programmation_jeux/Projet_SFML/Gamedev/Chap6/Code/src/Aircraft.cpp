#include <Aircraft.h>
#include <ResourceHolder.hpp>
#include <Category.h>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


/*
Permet d'acc�der � l'ID de la texture de l'avion gr�ce aux mots Eagle ou Raptor
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
type = namespace contenant les noms des textures pouvant �tre associ�s aux Aircraft.
m_type = attribut de type
textures = Passage en r�f�rence de la class TextureHolder contenant l'ensemble des textures du jeux.
m_sprite = r�cup�re le sprite de la texture du TextureHolder d�finit par "type"(Eagle ou Raptor).
            "textures.get(toTextureID(type))" r�cup�re le pointeur de la class texture associ� � l'ID "type".
            La d�claration "sf::Sprite" de m_sprite associe m_sprite � la sous-classe de Texture nomm� Sprite.
*/
Aircraft::Aircraft(Type type,const TextureHolder& textures) : m_type(type), m_sprite(textures.get(toTextureID(type)))
{
    sf::FloatRect bounds = m_sprite.getLocalBounds(); // m�thode de Sprite r�cup�rant la position de l'image.
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f); // m�thode de sprite d�finissant la position de l'image.
}

/*
Permet de dessiner les textures d'aircraft sur l'�cran.

"RenderTarget" est une class de SFML qui permet de donner la fen�tre o� doit �tre dessiner la figure.
"RenderStates" est une class de SFML qui permet de donner les transformations du sprite (rotaton; shader, etc...)
*/
void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite,states);
}

/*
Permet d'acc�der � la cat�gory du Aircraft en fonction de l'ID de la texture(Eagle ou Raptor).
Cela renvoit un entier diff�rent en fonction de la cat�gorie, qui est d�finit dans Cat�gory.h
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


