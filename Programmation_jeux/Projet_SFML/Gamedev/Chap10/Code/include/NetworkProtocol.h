#ifndef NETWORKPROTOCOL_H_INCLUDED
#define NETWORKPROTOCOL_H_INCLUDED

#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>

const unsigned short ServerPort = 5000;

namespace Server
{
    enum PacketType
    {
        BroadcastMessage,
        SpawnSelf,
        InitialState,
        PlayerEvent,
        PlayerRealtimeChange,
        PlayerConnect,
        PlayerDisconnect,
        AcceptCoopPartner,
        SpawnEnemy,
        SpawnPickup,
        UpdateClientState,
        MissionSuccess
    };

}

amespace Client
{
	// Packets originated in the client
	enum PacketType
	{
		PlayerEvent,
		PlayerRealtimeChange,
		RequestCoopPartner,
		PositionUpdate,
		GameEvent,
		Quit
	};
}

namespace PlayerActions
{
	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Fire,
		LaunchMissile,
		ActionCount
	};
}

namespace GameActions
{
	enum Type
	{
		EnemyExplode,
	};

	struct Action
	{
		Action()
		{ // leave uninitialized
		}

		Action(Type type, sf::Vector2f position)
		: type(type)
		, position(position)
		{
		}

		Type			type;
		sf::Vector2f	position;
	};
}

#endif // BOOK_NETWORKPROTOCOL_HPP


#endif // NETWORKPROTOCOL_H_INCLUDED
