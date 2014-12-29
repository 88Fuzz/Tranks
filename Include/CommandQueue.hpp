#ifndef __COMMANDQUEUE_HPP__
#define __COMMANDQUEUE_HPP__

#include <Command.hpp>
#include <queue>

class CommandQueue
{
public:
    void push(Command *);
    Command* pop();
    bool isEmpty() const;

private:
    std::queue<Command *> queue;
};

#endif
