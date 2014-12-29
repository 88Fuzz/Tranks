#include <CommandQueue.hpp>
#include <SceneNode.hpp>

void CommandQueue::push(Command *command)
{
    queue.push(command);
}

Command *CommandQueue::pop()
{
    Command *command = queue.front();
    queue.pop();
    return command;
}

bool CommandQueue::isEmpty() const
{
    return queue.empty();
}
