#ifndef MULTIPLAYERGAMESTATE_H
#define MULTIPLAYERGAMESTATE_H

#include <State.h>
#include <World.h>
#include <Player.h>
#include <GameServer.h>
#include <NetworkProtocol.h>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>


class MultiplayerGameState : public State
{
public:
    MultiplayerGameState(StateStack& stack, Context context, bool isHost);
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    virtual void onActivate();
    void onDestroy();

    void disableAllRealtimeActions();

private:
    void updateBroadcastMessage(sf::Time elapsedTime);
    void handlePacket(sf::Int32 packetType, sf::Packet& packet);

private:
    typedef std::unique_ptr<Player> PlayerPtr;

private:
    World m_world;
    sf::RenderWindow& m_window;
    TextureHolder& m_textureHolder;

    std::map<int, PlayerPtr> m_players;
    std::vector<sf::Int32> m_localPlayerIdentifiers;
    sf::TcpSocket m_socket;
    bool m_connected;
    std::unique_ptr<GameServer> m_gameServer;
    sf::Clock m_tickClock;

    std::vector<std::string> m_broadcasts;
    sf::Text m_broadcastText;
    sf::Time m_broadcastElapsedTime;

    sf::Text m_playerInvitationText;
    sf::Time m_playerInvitationTime;

    sf::Text m_failedConnectionText;
    sf::Clock m_failedConnectionClock;

    bool m_activeState;
    bool m_hasFocus;
    bool m_host;
    bool m_gameStarted;
    sf::Time m_clientTimeout;
    sf::Time m_timeSinceLastPacket;
};

#endif // MULTIPLAYERGAMESTATE_H
