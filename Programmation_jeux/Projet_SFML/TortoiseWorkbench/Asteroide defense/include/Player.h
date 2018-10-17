#ifndef PLAYER_H
#define PLAYER_H

#include <Command.h>
#include <KeyBinding.h>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <map>

class CommandQueue;

/*
    Permet de gérer les commandesd'entrée du joeur
*/
class Player : private sf::NonCopyable
{
public:
    typedef PlayerAction::Type Action;

    //liste l'ensemble des actions possible par le joueur
    enum MissionStatus
    {
        MissionRunning,
        MissionSuccess,
        MissionFailure
    };


public:
    Player (sf::TcpSocket* socket, sf::Int32 identifier,
            const KeyBinding* binding);

    void handleEvent (const sf::Event& event, CommandQueue& commands);
    void handleRealtimeInput (CommandQueue& commands);
    void handleRealtimeNetworkInput (CommandQueue& commands);

    void handleNetworkEvent (Action action, CommandQueue& commands);
    void handleNetworkRealtimeChange (Action action, bool actionEnabled);

    void setMissionStatus (MissionStatus status);
    MissionStatus getMissionStatus() const;

    void disableAllRealtimeActions();
    bool isLocal() const;

private:
    void initializeActions();

private:
    const KeyBinding* m_keyBinding;
    std::map<Action, Command> m_actionBinding;
    std::map<Action, bool> m_actionProxies;
    MissionStatus m_currentMissionStatus;
    int m_identifier;
    sf::TcpSocket* m_socket;
};

#endif // PLAYER_H
