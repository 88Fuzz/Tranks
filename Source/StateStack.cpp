#include "StateStack.hpp"
#include <iostream>

StateStack::StateStack(State::Context context) :
        stateStack(), actionList(), context(context), factories()
{
}

void StateStack::update(sf::Time dt)
{
    // Iterate from top to bottom, stop as soon as update() returns false
    //TODO figure out why this will be terminated by a false
    for(auto itr = stateStack.rbegin(); itr != stateStack.rend(); ++itr)
    {
        if(!(*itr)->update(dt))
            break;
    }

    applyPendingChanges();
}

void StateStack::draw()
{
    // Draw all active states from bottom to top
    for(std::vector<State *>::iterator it = stateStack.begin(); it != stateStack.end(); it++)
    {
        (*it)->draw();
    }
}

void StateStack::handleEvent(const sf::Event* event)
{
    // Iterate from top to bottom, stop as soon as handleEvent() returns false
    for(auto itr = stateStack.rbegin(); itr != stateStack.rend(); ++itr)
    {
        if(!(*itr)->handleEvent(event))
            break;
    }

    applyPendingChanges();
}

void StateStack::pushState(States::ID stateID)
{
    actionList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
    actionList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
    actionList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
    return stateStack.empty();
}

State* StateStack::createState(States::ID stateID)
{
    //TODO error check
    auto found = factories.find(stateID);

    return found->second();
}

void StateStack::applyPendingChanges()
{
    for(std::vector<PendingChange>::iterator it = actionList.begin(); it != actionList.end(); it++)
    {
        switch((*it).action)
        {
        case Push:
            stateStack.push_back(createState((*it).stateID));
            break;

        case Pop:
            delete stateStack.back();
            stateStack.pop_back();
            break;

        case Clear:
            for(std::vector<State *>::iterator it = stateStack.begin(); it != stateStack.end(); it++)
                delete *it;
            stateStack.clear();
            break;
        }
    }

    actionList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID) :
        action(action), stateID(stateID)
{
}
