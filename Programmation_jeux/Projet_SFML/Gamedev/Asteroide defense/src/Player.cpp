#include <Player.h>
#include <CommandQueue.h>
#include <Aircraft.h>
#include <Foreach.hpp>
#include <NetworkProtocol.h>

#include <SFML/Network/Packet.hpp>

#include <map>
#include <string>
#include <algorithm>

using namespace std::placeholders;
/*
Applique les mouvements des entrée clavier à l'avion du joueur
*/
struct AircraftMover
{
    //applique la vitesse à l'avion
    AircraftMover(float vx, float vy, int identifier)
        : velocity(vx, vy)
        , aircraftID(identifier)
    {
    }

    //Surcharge de l'opérateur() pour augmenter l'accélération de la vitesse
    void operator() (Aircraft& aircraft, sf::Time) const
    {
        if (aircraft.getIdentifier() == aircraftID)
            aircraft.accelerate(velocity * aircraft.getMaxSpeed());
    }

    sf::Vector2f velocity;
    int aircraftID;
};

struct AircraftFireTrigger
{
    AircraftFireTrigger(int identifier)
        : aircraftID(identifier)
    {
    }

    void operator() (Aircraft& aircraft, sf::Time) const
    {
        if (aircraft.getIdentifier() == aircraftID)
            aircraft.fire();
    }

    int aircraftID;
};

struct AircraftMissileTrigger
{
    AircraftMissileTrigger(int identifier)
        : aircraftID(identifier)
    {
    }

    void operator() (Aircraft& aircraft, sf::Time) const
    {
        if (aircraft.getIdentifier() == aircraftID)
            aircraft.launchMissile();
    }

    int aircraftID;
};

Player::Player(sf::TcpSocket* socket, sf::Int32 identifier, const KeyBinding* binding)
    : m_keyBinding(binding)
    , m_currentMissionStatus(MissionRunning)
    , m_identifier(identifier)
    , m_socket(socket)
{
    //Définit l'action à associé à l'entrée clavier
    initializeActions();

    // Assign all categories to player's aircraft
    FOREACH(auto& pair, m_actionBinding)
    pair.second.category = Category::PlayerAircraft;
}

/*
Vérifie que c'est une entrée clavier et que l'action n'est pas une action en temps réel.
L'action sera effectuer une fois lors de l'entrée clavié et non à chaque frame comme handle realtime input.
*/
void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if (event.type == sf::Event::KeyPressed)
    {
        Action action;
        if (m_keyBinding && m_keyBinding->checkAction(event.key.code, action) && !isRealtimeAction(action))
        {
            if (m_socket)
            {
                sf::Packet packet;
                packet << static_cast<sf::Int32>(Client::PlayerEvent);
                packet << m_identifier;
                packet << static_cast<sf::Int32>(action);
                m_socket->send(packet);
            }

            else
            {
                commands.push(m_actionBinding[action]);
            }
        }
    }

    if ((event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) && m_socket)
    {
        Action action;
        if (m_keyBinding && m_keyBinding->checkAction(event.key.code, action) && isRealtimeAction(action))
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32>(Client::PlayerRealtimeChange);
            packet << m_identifier;
            packet << static_cast<sf::Int32>(action);
            packet << (event.type == sf::Event::KeyPressed);
            m_socket->send(packet);
        }
    }
}

bool Player::isLocal() const
{
    return m_keyBinding != nullptr;
}

void Player::disableAllRealtimeActions()
{
    FOREACH(auto& action, m_actionProxies)
    {
        sf::Packet packet;
        packet << static_cast<sf::Int32>(Client::PlayerRealtimeChange);
        packet << m_identifier;
        packet << static_cast<sf::Int32>(action.first);
        packet << false;
        m_socket->send(packet);
    }
}

/*
met la commande du joueur dans la liste des commande à appliquer à cette frame
*/
void Player::handleRealtimeInput(CommandQueue& commands)
{
    if ((m_socket && isLocal()) || !m_socket)
    {
        std::vector<Action> activeActions = m_keyBinding->getRealtimeActions();
        FOREACH(Action action, activeActions)
        commands.push(m_actionBinding[action]);
    }
}

void Player::handleRealtimeNetworkInput(CommandQueue& commands)
{
    if (m_socket && !isLocal())
    {
        FOREACH(auto pair, m_actionProxies)
        {
            if (pair.second && isRealtimeAction(pair.first))
                commands.push(m_actionBinding[pair.first]);
        }
    }
}

void Player::handleNetworkEvent(Action action, CommandQueue& commands)
{
    commands.push(m_actionBinding[action]);
}

void Player::handleNetworkRealtimeChange(Action action, bool actionEnabled)
{
    m_actionProxies[action] = actionEnabled;
}

void Player::setMissionStatus(MissionStatus status)
{
    m_currentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
    return m_currentMissionStatus;
}

/*
Définit les actions induite par les commandes d'entrée(clavier)
*/
void Player::initializeActions()
{
    //définit les actions à réaliser des entrée clavier sur l'avion du joueur. Doit coller avec isRealtimeAction.
    m_actionBinding[PlayerAction::MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-1, 0, m_identifier));
    m_actionBinding[PlayerAction::MoveRight].action = derivedAction<Aircraft>(AircraftMover(+1, 0, m_identifier));
    m_actionBinding[PlayerAction::MoveUp].action = derivedAction<Aircraft>(AircraftMover(0, -1, m_identifier));
    m_actionBinding[PlayerAction::MoveDown].action = derivedAction<Aircraft>(AircraftMover(0, +1, m_identifier));
    m_actionBinding[PlayerAction::Fire].action = derivedAction<Aircraft>(AircraftFireTrigger(m_identifier));
    m_actionBinding[PlayerAction::LaunchMissile].action = derivedAction<Aircraft>(AircraftMissileTrigger(m_identifier));
}
