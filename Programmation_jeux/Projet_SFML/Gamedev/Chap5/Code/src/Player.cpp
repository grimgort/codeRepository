#include <Player.h>
#include <CommandQueue.h>
#include <Aircraft.h>
#include <Foreach.hpp>

#include <map>
#include <string>
#include <algorithm>

/*
Applique les mouvements des entr�e clavier � l'avion du joueur
*/
struct AircraftMover
{
    //applique la vitesse � l'avion
    AircraftMover(float vx, float vy) : velocity(vx, vy)
    {
    }

    //Surcharge de l'op�rateur() pour augmenter l'acc�l�ration de la vitesse
    void operator() (Aircraft& aircraft, sf::Time) const
    {
        aircraft.accelerate(velocity);
    }

    sf::Vector2f velocity;
};

Player::Player()
{
    //On rajoute les codes clavier utilisable par le joueur dans un tableau map.
    //doit coller avec isRealtimeAction
    m_keyBinding[sf::Keyboard::Left] = MoveLeft;
    m_keyBinding[sf::Keyboard::Right] = MoveRight;
    m_keyBinding[sf::Keyboard::Up] = MoveUp;
    m_keyBinding[sf::Keyboard::Down] = MoveDown;

    //D�finit l'action � associ� � l'entr�e clavier
    initializeActions();

	// Assign all categories to player's aircraft
	FOREACH(auto& pair, m_actionBinding)
	{
	    pair.second.category = Category::PlayerAircraft;
	}
}

/*
V�rifie que c'est une entr�e clavier et que l'action n'est pas une action en temps r�el.
L'action sera effectuer une fois lors de l'entr�e clavi� et non � chaque frame comme handle realtime input.
*/
void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if (event.type == sf::Event::KeyPressed)
    {
        auto found = m_keyBinding.find(event.key.code);
        if (found != m_keyBinding.end() && !isRealtimeAction(found->second))
        {
            //On rajoute la commande aux action a effectuer
            commands.push(m_actionBinding[found->second]);
        }
    }
}

/*
met la commande du joueur dans la liste des commande � appliquer � cette frame
*/
void Player::handleRealtimeInput(CommandQueue& commands)
{
    //
	FOREACH(auto pair, m_keyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		//si le code de la touche press� correspond � la liste des commande on rajoute la commande dans la liste
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
        {
            commands.push(m_actionBinding[pair.second]);
        }
	}
}

/*
Assigne une action � une cl�s clavier (semble obsolette)
*/
void Player::assignKey(Action action, sf::Keyboard::Key key)
{
    for (auto itr = m_keyBinding.begin(); itr != m_keyBinding.end();)
    {
        if (itr->second == action)
        {
            m_keyBinding.erase(itr++);
        }
        else
        {
            ++itr;
        }
    }
    m_keyBinding[key] = action;
}
/*
R�cup�re la cl�s de l'action.
*/
sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	FOREACH(auto pair, m_keyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}
    return sf::Keyboard::Unknown;
}

/*
D�finit les actions induite par les commandes d'entr�e(clavier)
*/
void Player::initializeActions()
{
    //vitesse du joueur
    const float playerSpeed = 200.f;

    //d�finit les actions � r�aliser des entr�e clavier sur l'avion du joueur. Doit coller avec isRealtimeAction.
    m_actionBinding[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
    m_actionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+playerSpeed, 0.f));
    m_actionBinding[MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f, -playerSpeed));
    m_actionBinding[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, +playerSpeed));
}

/*
Liste les comandes possible par le joueur
*/
bool Player::isRealtimeAction(Action action)
{
    switch (action)
    {
    case MoveLeft:
    case MoveRight:
    case MoveDown:
    case MoveUp:
        return true;
    default:
        return false;
    }
}
