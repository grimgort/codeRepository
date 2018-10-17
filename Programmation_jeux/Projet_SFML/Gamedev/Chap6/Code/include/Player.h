#ifndef PLAYER_H
#define PLAYER_H

#include <Command.h>
#include <CommandQueue.h>

#include <SFML/Window/Event.hpp>

#include <map>

/*
Permet de gérer les commandesd'entrée du joeur
*/
class Player
{
	public:
        //liste l'ensemble des actions possible par le joueur
		enum Action
		{
			MoveLeft,
			MoveRight,
			MoveUp,
			MoveDown,
			ActionCount
		};
	public:
		Player();

		void handleEvent(const sf::Event& event, CommandQueue& commands);
		void handleRealtimeInput(CommandQueue& commands);

		void assignKey(Action action, sf::Keyboard::Key key);
		sf::Keyboard::Key getAssignedKey(Action action) const;
    private:
        void initializeActions();
		static bool isRealtimeAction(Action action);

    private:
        std::map<sf::Keyboard::Key, Action> m_keyBinding;
        std::map<Action, Command> m_actionBinding;
};

#endif // PLAYER_H
