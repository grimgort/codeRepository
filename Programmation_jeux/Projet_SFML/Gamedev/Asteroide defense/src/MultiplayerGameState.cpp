#include "MultiplayerGameState.h"
#include <MusicPlayer.h>
#include <Foreach.hpp>
#include <Utility.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/IpAddress.hpp>

#include <fstream>

sf::IpAddress getAddressFromFile()
{
    {
        std::ifstream inputFile("ip.txt");
        std::string ipAddress;
        if (inputFile >> ipAddress)
            return ipAddress;
    }

    std::ofstream outputFile("ip.txt");
    std::string localAddress = "127.0.0.1";
    outputFile << localAddress;
    return localAddress;
}


MultiplayerGameState::MultiplayerGameState(StateStack& stack, Context context, bool isHost)
    : State(stack, context)
    , m_world(*context.window, *context.fonts, *context.sounds, true)
    , m_window(*context.window)
    , m_textureHolder(*context.textures)
    , m_connected(false)
    , m_gameServer(nullptr)
    , m_activeState(true)
    , m_hasFocus(true)
    , m_host(isHost)
    , m_gameStarted(false)
    , m_clientTimeout(sf::seconds(2.f))
    , m_timeSinceLastPacket(sf::seconds(0.f))
{
    m_broadcastText.setFont(context.fonts->get(Fonts::Main));
    m_broadcastText.setPosition(1024.f / 2, 100.f);

    m_playerInvitationText.setFont(context.fonts->get(Fonts::Main));
    m_playerInvitationText.setCharacterSize(20);
    m_playerInvitationText.setColor(sf::Color::White);
    m_playerInvitationText.setString("Press Enter to spawn player 2");
    m_playerInvitationText.setPosition(1000 - m_playerInvitationText.getLocalBounds().width, 760 - m_playerInvitationText.getLocalBounds().height);

    m_failedConnectionText.setFont(context.fonts->get(Fonts::Main));
    m_failedConnectionText.setString("Attempting to connect...");
    m_failedConnectionText.setCharacterSize(35);
    m_failedConnectionText.setColor(sf::Color::White);
    centerOrigin(m_failedConnectionText);
    m_failedConnectionText.setPosition(m_window.getSize().x / 2.f,  m_window.getSize().y / 2.f);

    m_window.clear(sf::Color::Black);
    m_window.draw(m_failedConnectionText);
    m_window.display();
    m_failedConnectionText.setString("Could not connect to the remote server!");
    centerOrigin(m_failedConnectionText);

    sf::IpAddress ip;
    if (isHost)
    {
        m_gameServer.reset(new GameServer(sf::Vector2f(m_window.getSize())));
        ip = "127.0.0.1";
    }
    else
    {
        ip = getAddressFromFile();
    }

    if (m_socket.connect(ip, ServerPort, sf::seconds(5.f)) == sf::TcpSocket::Done)
        m_connected = true;
    else
        m_failedConnectionClock.restart();

    m_socket.setBlocking(false);

    context.music->play(Music::MissionTheme);
}

void MultiplayerGameState::draw()
{
    if (m_connected)
    {
        m_world.draw();

        m_window.setView(m_window.getDefaultView());

        if (!m_broadcasts.empty())
            m_window.draw(m_broadcastText);

        if (m_localPlayerIdentifiers.size() < 2 && m_playerInvitationTime < sf::seconds(0.5f))
            m_window.draw(m_playerInvitationText);
    }
    else
    {
        m_window.draw(m_failedConnectionText);
    }
}

void MultiplayerGameState::onActivate()
{
    m_activeState = true;
}

void MultiplayerGameState::onDestroy()
{
    if (!m_host && m_connected)
    {
        sf::Packet packet;
        packet << static_cast<sf::Int32>(Client::Quit);
        m_socket.send(packet);
    }

}

bool MultiplayerGameState::update(sf::Time dt)
{
    if (m_connected)
    {
        m_world.update(dt);

        bool foundLocalPlane = false;
        for (auto itr = m_players.begin(); itr != m_players.end(); )
        {
            if (std::find(m_localPlayerIdentifiers.begin(), m_localPlayerIdentifiers.end(), itr->first) != m_localPlayerIdentifiers.end())
            {
                foundLocalPlane = true;
            }

            if (!m_world.getAircraft(itr->first))
            {
                itr = m_players.erase(itr);

                if (m_players.empty())
                    requestStackPush(States::GameOver);
            }
            else
            {
                ++itr;
            }
        }

        if (!foundLocalPlane && m_gameStarted)
        {
            requestStackPush(States::GameOver);
        }

        if (m_activeState && m_hasFocus)
        {
            CommandQueue& commands = m_world.getCommandQueue();
            FOREACH(auto& pair, m_players)
            pair.second->handleRealtimeInput(commands);
        }
        CommandQueue& commands = m_world.getCommandQueue();
        FOREACH(auto& pair, m_players)
        pair.second->handleRealtimeNetworkInput(commands);

        sf::Packet packet;
        if (m_socket.receive(packet) == sf::Socket::Done)
        {
            m_timeSinceLastPacket = sf::seconds(0.f);
            sf::Int32 packetType;
            packet >> packetType;
            handlePacket(packetType, packet);
        }
        else
        {
            if (m_timeSinceLastPacket > m_clientTimeout)
            {
                m_connected = false;

                m_failedConnectionText.setString("Lost connection to server");
                centerOrigin(m_failedConnectionText);

                m_failedConnectionClock.restart();
            }
        }

        updateBroadcastMessage(dt);

        m_playerInvitationTime += dt;
        if (m_playerInvitationTime > sf ::seconds(1.f))
            m_playerInvitationTime = sf::Time::Zero;

        GameActions::Action gameAction;
        while (m_world.pollGameAction(gameAction))
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32>(Client::GameEvent);
            packet << static_cast<sf::Int32>(gameAction.type);
            packet << gameAction.position.x;
            packet << gameAction.position.y;

            m_socket.send(packet);
        }

        if (m_tickClock.getElapsedTime() > sf::seconds(1.f / 20.f))
        {
            sf::Packet positionUpdatePacket;
            positionUpdatePacket << static_cast<sf::Int32>(Client::PositionUpdate);
            positionUpdatePacket << static_cast<sf::Int32>(m_localPlayerIdentifiers.size());

            FOREACH(sf::Int32 identifier, m_localPlayerIdentifiers)
            {
                if (Aircraft* aircraft = m_world.getAircraft(identifier))
                    positionUpdatePacket << identifier << aircraft->getPosition().x << aircraft->getPosition().y <<
                                         static_cast<sf::Int32>(aircraft->getHitpoints()) << static_cast<sf::Int32>(aircraft->getMissileAmmo());
            }
            m_socket.send(positionUpdatePacket);
            m_tickClock.restart();
        }
        m_timeSinceLastPacket += dt;
    }
    else if (m_failedConnectionClock.getElapsedTime() >= sf::seconds(5.f))
    {
        requestStateClear();
        requestStackPush(States::Menu);
    }

    return true;
}

void MultiplayerGameState::disableAllRealtimeActions()
{
    m_activeState = false;

    FOREACH(sf::Int32 identifier, m_localPlayerIdentifiers)
    m_players[identifier]->disableAllRealtimeActions();
}

bool MultiplayerGameState::handleEvent(const sf::Event& event)
{
    CommandQueue& commands = m_world.getCommandQueue();

    FOREACH (auto& pair, m_players)
    pair.second->handleEvent(event, commands);

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Return && m_localPlayerIdentifiers.size() == 1)
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32>(Client::RequestCoopPartner);

            m_socket.send(packet);
        }

        else if (event.key.code == sf::Keyboard::Escape)
        {
            disableAllRealtimeActions();
            requestStackPush(States::NetworkPause);
        }
    }
    else if (event.type == sf::Event::GainedFocus)
    {
        m_hasFocus = true;
    }
    else if (event.type == sf::Event::LostFocus)
    {
        m_hasFocus = false;
    }

    return true;
}

void MultiplayerGameState::updateBroadcastMessage(sf::Time elapsedTime)
{
    if (m_broadcasts.empty())
        return;

    m_broadcastElapsedTime += elapsedTime;
    if (m_broadcastElapsedTime > sf::seconds(2.5f))
    {
        m_broadcasts.erase(m_broadcasts.begin());

        if (!m_broadcasts.empty())
        {
            m_broadcastText.setString(m_broadcasts.front());
            centerOrigin(m_broadcastText);
            m_broadcastElapsedTime = sf::Time::Zero;
        }
    }
}

void MultiplayerGameState::handlePacket(sf::Int32 packetType, sf::Packet& packet)
{
    switch (packetType)
    {
    case Server::BroadcastMessage:
    {
        std::string message;
        packet >> message;
        m_broadcasts.push_back(message);

        if (m_broadcasts.size() == 1)
        {
            m_broadcastText.setString(m_broadcasts.front());
            centerOrigin(m_broadcastText);
            m_broadcastElapsedTime = sf::Time::Zero;
        }
    }
    break;

    case Server::SpawnSelf:
    {
        sf::Int32 aircraftIdentifier;
        sf::Vector2f aircraftPosition;
        packet >> aircraftIdentifier >> aircraftPosition.x >> aircraftPosition.y;

        Aircraft* aircraft = m_world.addAircraft(aircraftIdentifier);
        aircraft->setPosition(aircraftPosition);

        m_players[aircraftIdentifier].reset(new Player(&m_socket, aircraftIdentifier, getContext().keys1));
        m_localPlayerIdentifiers.push_back(aircraftIdentifier);

        m_gameStarted = true;
    }
    break;

    case Server::PlayerConnect:
    {
        sf::Int32 aircraftIdentifier;
        sf::Vector2f aircraftPosition;
        packet >> aircraftIdentifier >> aircraftPosition.x >> aircraftPosition.y;

        Aircraft* aircraft = m_world.addAircraft(aircraftIdentifier);
        aircraft->setPosition(aircraftPosition);

        m_players[aircraftIdentifier].reset(new Player(&m_socket, aircraftIdentifier, nullptr));
    }
    break;

    case Server::PlayerDisconnect:
    {
        sf::Int32 aircraftIdentifier;
        packet >> aircraftIdentifier;

        m_world.removeAircraft(aircraftIdentifier);
        m_players.erase(aircraftIdentifier);
    }
    break;

    case Server::InitialState:
    {
        sf::Int32 aircraftCount;
        float worldHeight, currentScroll;
        packet >> worldHeight >> currentScroll;

        m_world.setWorldHeight(worldHeight);
        m_world.setCurrentBattleFieldPosition(currentScroll);

        packet >> aircraftCount;
        for (sf::Int32 i = 0; i < aircraftCount; ++i)
        {
            sf::Int32 aircraftIdentifier;
            sf::Int32 hitpoints;
            sf::Int32 missileAmmo;
            sf::Vector2f aircraftPosition;
            packet >> aircraftIdentifier >> aircraftPosition.x >> aircraftPosition.y >> hitpoints >> missileAmmo;

            Aircraft* aircraft = m_world.addAircraft(aircraftIdentifier);
            aircraft->setPosition(aircraftPosition);
            aircraft->setHitpoints(hitpoints);
            aircraft->setMissileAmmo(missileAmmo);

            m_players[aircraftIdentifier].reset(new Player(&m_socket, aircraftIdentifier, nullptr));
        }
    }
    break;

    case Server::AcceptCoopPartner:
    {
        sf::Int32 aircraftIdentifier;
        packet >> aircraftIdentifier;

        m_world.addAircraft(aircraftIdentifier);
        m_players[aircraftIdentifier].reset(new Player(&m_socket, aircraftIdentifier, getContext().keys2));
        m_localPlayerIdentifiers.push_back(aircraftIdentifier);
    }
    break;

    case Server::PlayerEvent:
    {
        sf::Int32 aircraftIdentifier;
        sf::Int32 action;
        packet >> aircraftIdentifier >> action;

        auto itr = m_players.find(aircraftIdentifier);
        if (itr != m_players.end())
            itr->second->handleNetworkEvent(static_cast<Player::Action>(action), m_world.getCommandQueue());
    }
    break;

    case Server::PlayerRealtimeChange:
    {
        sf::Int32 aircraftIdentifier;
        sf::Int32 action;
        bool actionEnabled;
        packet >> aircraftIdentifier >> action >> actionEnabled;

        auto itr = m_players.find(aircraftIdentifier);
        if (itr != m_players.end())
            itr->second->handleNetworkRealtimeChange(static_cast<Player::Action>(action), actionEnabled);
    }
    break;
    case Server::SpawnEnemy:
    {
        float height;
        sf::Int32 type;
        float relativeX;
        packet >> type >> height >> relativeX;

        m_world.addEnemy(static_cast<Aircraft::Type>(type), relativeX, height);
        m_world.sortEnemies();
    }
    break;

    case Server::MissionSuccess:
    {
        requestStackPush(States::MissionSuccess);
    }
    break;

    case Server::SpawnPickup:
    {
        sf::Int32 type;
        sf::Vector2f position;
        packet >> type >> position.x >> position.y;

        m_world.createPickup(position, static_cast<Pickup::Type>(type));
    }
    break;

    case Server::UpdateClientState:
    {
        float currentWorldPosition;
        sf::Int32 aircraftCount;
        packet >> currentWorldPosition >> aircraftCount;

        float currentViewPosition = m_world.getViewBounds().top + m_world.getViewBounds().height;

        m_world.setWorldScrollCompensation(currentViewPosition / currentWorldPosition);

        for (sf::Int32 i = 0; i < aircraftCount; i++)
        {
            sf::Vector2f aircraftPosition;
            sf::Int32 aircraftIdentifier;
            packet >> aircraftIdentifier >> aircraftPosition.x >> aircraftPosition.y;

            Aircraft* aircraft = m_world.getAircraft(aircraftIdentifier);
            bool isLocalPlane = std::find(m_localPlayerIdentifiers.begin(), m_localPlayerIdentifiers.end(), aircraftIdentifier) != m_localPlayerIdentifiers.end();
            if (aircraft && !isLocalPlane)
            {
                sf::Vector2f interpolatedPosition = aircraft->getPosition() + (aircraftPosition - aircraft->getPosition()) * 0.1f;
                aircraft->setPosition(interpolatedPosition);
            }
        }
    }
    break;
    }
}
