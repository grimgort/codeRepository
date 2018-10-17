#ifndef STATEIDENTIFIERS_H_INCLUDED
#define STATEIDENTIFIERS_H_INCLUDED

namespace States
{
	enum ID
	{
		None,
		Title,
		Menu,
		Game,
		Loading,
		Pause,
		NetworkPause,
		Settings,
		GameOver,
		MissionSuccess,
		HostGame,
		JoinGame,
	};
}

#endif // STATEIDENTIFIERS_H_INCLUDED
