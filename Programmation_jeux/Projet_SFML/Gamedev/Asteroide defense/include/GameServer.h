#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <vector>
#include <memory>
#include <map>


class GameServer
{
public:
    explicit GameServer(sf::Vector2f battlefieldSize);
    ~GameServer();

    void notifyPlayerSpawn(sf::Int32 aircraftIdentifier);
    void notifyPlayerRealtimeChange(sf::Int32 aircraftIdentifier, sf::Int32 action, bool actionEnable);
    void notifyPlayerEvent(sf::Int32 aircraftIdentifier, sf::Int32 action);

private:
    struct RemotePeer
    {
        RemotePeer();
        sf::TcpSocket socket;
        sf::Time lastPacketTime;
        std::vector<sf::Int32> aircraftIdentifiers;
        bool ready;
        bool timedOut;
    };

    struct AircraftInfo
    {
        sf::Vector2f position;
        sf::Int32 hitpoints;
        sf::Int32 missileAmmo;
        std::map<sf::Int32, bool> realtimeActions;
    };

    typedef std::unique_ptr<RemotePeer> PeerPtr;

private:
    void setListening(bool enable);
    void executionThread();
    void tick();
    sf::Time now() const;

    void handleIncomingPackets();
    void handleIncomingPacket(sf::Packet& packet, RemotePeer& receivingPeer, bool& detectedTimeout);

    void handleIncomingConnections();
    void handleDisconnections();

    void informWorldState(sf::TcpSocket& socket);
    void broadcastMessage(const std::string& message);
    void sendToAll(sf::Packet& packet);
    void updateClientState();

private:
    sf::Thread m_thread;
    sf::Clock m_clock;
    sf::TcpListener m_listenerSocket;
    bool m_listeningState;
    sf::Time m_clientTimeoutTime;

    std::size_t m_maxConnectedPlayers;
    std::size_t m_connectedPlayers;

    float m_worldHeight;
    sf::FloatRect m_battleFieldRect;
    float m_battleFieldScrollSpeed;

    std::size_t m_aircraftCount;
    std::map<sf::Int32, AircraftInfo> m_aircraftInfo;

    std::vector<PeerPtr> m_peers;
    sf::Int32 m_aircraftIdentifierCounter;
    bool m_waitingThreadEnd;

    sf::Time m_lastSpawnTime;
    sf::Time m_timeForNextSpawn;
};

#endif // GAMESERVER_H
