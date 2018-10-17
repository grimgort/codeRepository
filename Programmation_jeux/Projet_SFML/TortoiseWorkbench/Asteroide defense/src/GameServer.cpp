#include "GameServer.h"
#include <NetworkProtocol.h>
#include <Foreach.hpp>
#include <Utility.h>
#include <Pickup.h>
#include <Aircraft.h>

#include <SFML/Network/Packet.hpp>


GameServer::RemotePeer::RemotePeer()
    : ready (false)
    , timedOut (false)
{
    socket.setBlocking (false);
}

GameServer::GameServer (sf::Vector2f battlefieldSize)
    : m_thread (&GameServer::executionThread, this)
    , m_listeningState (false)
    , m_clientTimeoutTime (sf::seconds (3.f))
    , m_maxConnectedPlayers (10)
    , m_connectedPlayers (0)
    , m_worldHeight (5000.f)
    , m_battleFieldRect (0.f, m_worldHeight - battlefieldSize.y,
                         battlefieldSize.x, battlefieldSize.y)
    , m_battleFieldScrollSpeed (-50.f)
    , m_aircraftCount (0)
    , m_peers (1)
    , m_aircraftIdentifierCounter (1)
    , m_waitingThreadEnd (false)
    , m_lastSpawnTime (sf::Time::Zero)
    , m_timeForNextSpawn (sf::seconds (5.f))
{
    m_listenerSocket.setBlocking (false);
    m_peers[0].reset (new RemotePeer());
    m_thread.launch();
}


GameServer::~GameServer()
{
    m_waitingThreadEnd = true;
    m_thread.wait();
}

void GameServer::notifyPlayerRealtimeChange (sf::Int32
        aircraftIdentifier, sf::Int32 action, bool actionEnabled)
{
    for (std::size_t i = 0; i < m_connectedPlayers; ++i)
    {
        if (m_peers[i]->ready)
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32> (Server::PlayerRealtimeChange);
            packet << aircraftIdentifier;
            packet << action;
            packet << actionEnabled;

            m_peers[i]->socket.send (packet);
        }
    }
}

void GameServer::notifyPlayerEvent (sf::Int32 aircraftIdentifier,
                                    sf::Int32 action)
{
    for (std::size_t i = 0; i < m_connectedPlayers; ++i)
    {
        if (m_peers[i]->ready)
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32> (Server::PlayerEvent);
            packet << aircraftIdentifier;
            packet << action;

            m_peers[i]->socket.send (packet);
        }
    }
}

void GameServer::notifyPlayerSpawn (sf::Int32 aircraftIdentifier)
{
    for (std::size_t i = 0; i < m_connectedPlayers; ++i)
    {
        if (m_peers[i]->ready)
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32> (Server::PlayerConnect);
            packet << aircraftIdentifier <<
                   m_aircraftInfo[aircraftIdentifier].position.x <<
                   m_aircraftInfo[aircraftIdentifier].position.y;
            m_peers[i]->socket.send (packet);
        }
    }
}

void GameServer::setListening (bool enable)
{
    if (enable)
    {
        if (!m_listeningState)
        { m_listeningState = (m_listenerSocket.listen (ServerPort) == sf::TcpListener::Done); }
    }
    else
    {
        m_listenerSocket.close();
        m_listeningState = false;
    }
}

void GameServer::executionThread()
{
    setListening (true);

    sf::Time stepInterval = sf::seconds (1.f / 60.f);
    sf::Time stepTime = sf::Time::Zero;
    sf::Time tickInterval = sf::seconds (1.f / 20.f);
    sf::Time tickTime = sf::Time::Zero;
    sf::Clock stepClock, tickClock;

    while (!m_waitingThreadEnd)
    {
        handleIncomingPackets();
        handleIncomingConnections();

        stepTime += stepClock.getElapsedTime();
        stepClock.restart();

        tickTime += tickClock.getElapsedTime();
        tickClock.restart();

        while (stepTime >= stepInterval)
        {
            m_battleFieldRect.top += m_battleFieldScrollSpeed *
                                     stepInterval.asSeconds();
            stepTime -= stepInterval;
        }

        while (tickTime >= tickInterval)
        {
            tick();
            tickTime -= tickInterval;
        }

        sf::sleep (sf::milliseconds (100));
    }
}

void GameServer::tick()
{
    updateClientState();

    //    bool allAircraftsDone = true;
    //    FOREACH(auto pair, m_aircraftInfo)
    //    {
    //        if (pair.second.position.y > 0.f)
    //            allAircraftsDone = false;
    //    }
    //    if (allAircraftsDone)
    //    {
    //        sf::Packet missionSuccessPacket;
    //        missionSuccessPacket << static_cast<sf::Int32>(Server::MissionSuccess);
    //        sendToAll(missionSuccessPacket);
    //    }

    for (auto itr = m_aircraftInfo.begin(); itr != m_aircraftInfo.end();)
    {
        if (itr->second.hitpoints <= 0)
        { m_aircraftInfo.erase (itr++); }
        else
        { ++itr; }
    }

    if (now() >= m_timeForNextSpawn + m_lastSpawnTime)
    {
        if (m_battleFieldRect.top > 600.f)
        {
            std::size_t enemyCount = 1u + randomInt (2);
            float spawnCenter = static_cast<float> (randomInt (500) - 250);

            float planeDistance = 0.f;
            float nextSpawnPosition = spawnCenter;

            if (enemyCount == 2)
            {
                planeDistance = static_cast<float> (150 + randomInt (250));
                nextSpawnPosition = spawnCenter - planeDistance / 2.f;
            }

            for (std::size_t i = 0; i < enemyCount; ++i)
            {
                sf::Packet packet;
                packet << static_cast<sf::Int32> (Server::SpawnEnemy);
                packet << static_cast<sf::Int32> (1 + randomInt (
                                                      Aircraft::TypeCount - 1));
                packet << m_worldHeight - m_battleFieldRect.top + 500;
                packet << nextSpawnPosition;

                nextSpawnPosition += planeDistance / 2.f;

                sendToAll (packet);
            }

            m_lastSpawnTime = now();
            m_timeForNextSpawn = sf::milliseconds (2000 + randomInt (6000));
        }
    }
}

sf::Time GameServer::now() const
{
    return m_clock.getElapsedTime();
}

void GameServer::handleIncomingPackets()
{
    bool detectedTimeout = false;

    FOREACH (PeerPtr & peer, m_peers)
    {
        if (peer->ready)
        {
            sf::Packet packet;

            while (peer->socket.receive (packet) == sf::Socket::Done)
            {
                handleIncomingPacket (packet, *peer, detectedTimeout);

                peer->lastPacketTime = now();
                packet.clear();
            }

            if (now() >= peer->lastPacketTime + m_clientTimeoutTime)
            {
                peer->timedOut = true;
                detectedTimeout = true;
            }
        }
    }

    if (detectedTimeout)
    { handleDisconnections(); }
}

void GameServer::handleIncomingPacket (sf::Packet& packet,
                                       RemotePeer& receivingPeer, bool& detectedTimeout)
{
    sf::Int32 packetType;
    packet >> packetType;

    switch (packetType)
    {
    case Client::Quit:
    {
        receivingPeer.timedOut = true;
        detectedTimeout = true;
    }
    break;

    case Client::PlayerEvent:
    {
        sf::Int32 aircraftIdentifier;
        sf::Int32 action;
        packet >> aircraftIdentifier >> action;

        notifyPlayerEvent (aircraftIdentifier, action);
    }
    break;

    case Client::PlayerRealtimeChange:
    {
        sf::Int32 aircraftIdentifier;
        sf::Int32 action;
        bool actionEnabled;
        packet >> aircraftIdentifier >> action >> actionEnabled;
        m_aircraftInfo[aircraftIdentifier].realtimeActions[action] =
            actionEnabled;
        notifyPlayerRealtimeChange (aircraftIdentifier, action,
                                    actionEnabled);
    }
    break;

    case Client::RequestCoopPartner:
    {
        receivingPeer.aircraftIdentifiers.push_back (
            m_aircraftIdentifierCounter);
        m_aircraftInfo[m_aircraftIdentifierCounter].position = sf::Vector2f (
                    m_battleFieldRect.width / 2,
                    m_battleFieldRect.top + m_battleFieldRect.height / 2);
        m_aircraftInfo[m_aircraftIdentifierCounter].hitpoints = 100;
        m_aircraftInfo[m_aircraftIdentifierCounter].missileAmmo = 2;

        sf::Packet requestPacket;
        requestPacket << static_cast<sf::Int32> (Server::AcceptCoopPartner);
        requestPacket << m_aircraftIdentifierCounter;
        requestPacket <<
                      m_aircraftInfo[m_aircraftIdentifierCounter].position.x;
        requestPacket <<
                      m_aircraftInfo[m_aircraftIdentifierCounter].position.y;

        receivingPeer.socket.send (requestPacket);
        m_aircraftCount++;

        FOREACH (PeerPtr & peer, m_peers)
        {
            if (peer.get() != &receivingPeer && peer->ready)
            {
                sf::Packet notifyPacket;
                notifyPacket << static_cast<sf::Int32> (Server::PlayerConnect);
                notifyPacket << m_aircraftIdentifierCounter;
                notifyPacket <<
                             m_aircraftInfo[m_aircraftIdentifierCounter].position.x;
                notifyPacket <<
                             m_aircraftInfo[m_aircraftIdentifierCounter].position.y;
                peer->socket.send (notifyPacket);
            }
        }
        m_aircraftIdentifierCounter++;
    }
    break;

    case Client::PositionUpdate:
    {
        sf::Int32 numAircrafts;
        packet >> numAircrafts;

        for (sf::Int32 i = 0; i < numAircrafts; ++i)
        {
            sf::Int32 aircraftIdentifier;
            sf::Int32 aircraftHitpoints;
            sf::Int32 missileAmmo;
            sf::Vector2f aircraftPosition;
            packet >> aircraftIdentifier >> aircraftPosition.x >>
                   aircraftPosition.y >> aircraftHitpoints >> missileAmmo;
            m_aircraftInfo[aircraftIdentifier].position = aircraftPosition;
            m_aircraftInfo[aircraftIdentifier].hitpoints = aircraftHitpoints;
            m_aircraftInfo[aircraftIdentifier].missileAmmo = missileAmmo;
        }
    }
    break;

    case Client::GameEvent:
    {
        sf::Int32 action;
        float x;
        float y;

        packet >> action;
        packet >> x;
        packet >> y;

        if (action == GameActions::EnemyExplode && randomInt (3) == 0
                && &receivingPeer == m_peers[0].get())
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32> (Server::SpawnPickup);
            packet << static_cast<sf::Int32> (randomInt (Pickup::TypeCount));
            packet << x;
            packet << y;

            sendToAll (packet);
        }
    }
    }
}

void GameServer::updateClientState()
{
    sf::Packet updateClientStatePacket;
    updateClientStatePacket << static_cast<sf::Int32>
                            (Server::UpdateClientState);
    updateClientStatePacket << static_cast<float> (m_battleFieldRect.top +
                            m_battleFieldRect.height);
    updateClientStatePacket << static_cast<sf::Int32>
                            (m_aircraftInfo.size());

    FOREACH (auto aircraft, m_aircraftInfo)
    updateClientStatePacket << aircraft.first <<
                            aircraft.second.position.x << aircraft.second.position.y;

    sendToAll (updateClientStatePacket);
}


void GameServer::handleIncomingConnections()
{
    if (!m_listeningState)
    { return; }

    if (m_listenerSocket.accept (m_peers[m_connectedPlayers]->socket) ==
            sf::TcpListener::Done)
    {
        m_aircraftInfo[m_aircraftIdentifierCounter].position = sf::Vector2f (
                    m_battleFieldRect.width / 2,
                    m_battleFieldRect.top + m_battleFieldRect.height / 2);
        m_aircraftInfo[m_aircraftIdentifierCounter].hitpoints = 100;
        m_aircraftInfo[m_aircraftIdentifierCounter].missileAmmo = 2;

        sf::Packet packet;
        packet << static_cast<sf::Int32> (Server::SpawnSelf);
        packet << m_aircraftIdentifierCounter;
        packet << m_aircraftInfo[m_aircraftIdentifierCounter].position.x;
        packet << m_aircraftInfo[m_aircraftIdentifierCounter].position.y;

        m_peers[m_connectedPlayers]->aircraftIdentifiers.push_back (
            m_aircraftIdentifierCounter);

        broadcastMessage ("New Player!");
        informWorldState (m_peers[m_connectedPlayers]->socket);
        notifyPlayerSpawn (m_aircraftIdentifierCounter++);

        m_peers[m_connectedPlayers]->socket.send (packet);
        m_peers[m_connectedPlayers]->ready = true;
        m_peers[m_connectedPlayers]->lastPacketTime = now();
        m_aircraftCount++;
        m_connectedPlayers++;

        if (m_connectedPlayers >= m_maxConnectedPlayers)
        { setListening (false); }
        else
        { m_peers.push_back (PeerPtr (new RemotePeer())); }
    }
}

void GameServer::handleDisconnections()
{
    for (auto itr = m_peers.begin(); itr != m_peers.end();)
    {
        if ( (*itr)->timedOut)
        {
            FOREACH (sf::Int32 identifier, (*itr)->aircraftIdentifiers)
            {
                sendToAll (sf::Packet() << static_cast<sf::Int32>
                           (Server::PlayerDisconnect) << identifier);

                m_aircraftInfo.erase (identifier);
            }

            m_connectedPlayers--;
            m_aircraftCount -= (*itr)->aircraftIdentifiers.size();

            itr = m_peers.erase (itr);

            if (m_connectedPlayers < m_maxConnectedPlayers)
            {
                m_peers.push_back (PeerPtr (new RemotePeer()));
                setListening (true);
            }

            broadcastMessage ("An ally has disconnected.");
        }
        else
        {
            ++itr;
        }
    }
}


void GameServer::informWorldState (sf::TcpSocket& socket)
{
    sf::Packet packet;
    packet << static_cast<sf::Int32> (Server::InitialState);
    packet << m_worldHeight << m_battleFieldRect.top +
           m_battleFieldRect.height;
    packet << static_cast<sf::Int32> (m_aircraftCount);

    for (std::size_t i = 0; i < m_connectedPlayers; ++i)
    {
        if (m_peers[i]->ready)
        {
            FOREACH (sf::Int32 identifier, m_peers[i]->aircraftIdentifiers)
            packet << identifier << m_aircraftInfo[identifier].position.x <<
                   m_aircraftInfo[identifier].position.y <<
                   m_aircraftInfo[identifier].hitpoints <<
                   m_aircraftInfo[identifier].missileAmmo;
        }
    }

    socket.send (packet);
}

void GameServer::broadcastMessage (const std::string& message)
{
    for (std::size_t i = 0; i < m_connectedPlayers; i++)
    {
        if (m_peers[i]->ready)
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32> (Server::BroadcastMessage);
            packet << message;

            m_peers[i]->socket.send (packet);
        }
    }
}

void GameServer::sendToAll (sf::Packet& packet)
{
    FOREACH (PeerPtr & peer, m_peers)
    {
        if (peer->ready)
        { peer->socket.send (packet); }
    }
}
