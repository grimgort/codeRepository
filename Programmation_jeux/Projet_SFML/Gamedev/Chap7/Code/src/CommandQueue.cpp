#include <CommandQueue.h>
#include <SceneNode.h>


/*
Rajoute la commande dans la queue des commande
*/
void CommandQueue::push(const Command& command)
{
	m_queue.push(command);
}

/*
Supprime la 1ère commande
*/
Command CommandQueue::pop()
{
	Command command = m_queue.front();
	m_queue.pop();
	return command;
}

/*
Renvoie true si la queue est vide
*/
bool CommandQueue::isEmpty() const
{
	return m_queue.empty();
}
