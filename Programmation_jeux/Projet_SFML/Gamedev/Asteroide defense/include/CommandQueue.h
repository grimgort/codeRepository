#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include <Command.h>
#include <queue>

/*
Crer et permet de gérer la liste des commande
*/
class CommandQueue
{
public:
    void push(const Command& command);
    Command pop();
    bool isEmpty() const;

private:
    std::queue<Command> m_queue;
};

#endif // COMMANDQUEUE_H
