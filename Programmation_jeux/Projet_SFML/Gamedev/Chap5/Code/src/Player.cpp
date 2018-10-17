#include <Player.h>
#include <CommandQueue.h>
#include <Aircraft.h>
#include <Foreach.hpp>

#include <map>
#include <string>
#include <algorithm>

/*
Applique les mouvements des entrée clavier à l'avion du joueur
*/
struct AircraftMover
{
    //applique la vitesse à l'avion
    AircraftMover(float vx, float vy) : velocity(vx, vy)
    {
    }

    //Surcharge de l'opérateur() pour augmenter l'accélération de la vitesse
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

    //Définit l'action à associé à l'entrée clavier
    initializeActions();

	// Assign all categories to player's aircraft
	FOREACH(auto& pair, m_actionBinding)
	{
	    pair.second.category = Category::PlayerAircraft;
	}
}

/*
Vérifie que c'est une entrée clavier et que l'action n'est pas une action en temps réel.
L'action sera effectuer une fois lors de l'entrée clavié et non à chaque frame comme handle realtime input.
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
met la commande du joueur dans la liste des commande à appliquer à cette frame
*/
void Player::handleRealtimeInput(CommandQueue& commands)
{
    //
	FOREACH(auto pair, m_keyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		//si le code de la touche pressé correspond à la liste des commande on rajoute la commande dans la liste
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
        {
            commands.push(m_actionBinding[pair.second]);
        }
	}
}

/*
Assigne une action à une clés clavier (semble obsolette)
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
Récupére la clés de l'action.
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
Définit les actions induite par les commandes d'entrée(clavier)
*/
void Player::initializeActions()
{
    //vitesse du joueur
    const float playerSpeed = 200.f;

    //définit les actions à réaliser des entrée clavier sur l'avion du joueur. Doit coller avec isRealtimeAction.
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
