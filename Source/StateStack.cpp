#include "StateStack.hpp"
#include "GameState.hpp"
#include <iostream>

StateStack::StateStack(State::Context context) :
        stateStack(), actionList(), context(context), factories()
{
}

void StateStack::update(sf::Time dt)
{
    // Iterate from top to bottom, stop as soon as update() returns false
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

void StateStack::swapState(States::ID stateID)
{
    actionList.push_back(PendingChange(Pop));
    actionList.push_back(PendingChange(Push, stateID));
}
void StateStack::swapState(States::ID stateId, int playerId, int numberOfPlayers, sf::TcpSocket *socket, GameServer *server)
{
    actionList.push_back(PendingChange(Pop));
    actionList.push_back(PendingChange(Push, stateId, playerId, numberOfPlayers, socket, server));
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

void StateStack::destroyState(State *state)
{
    delete state;
}

void StateStack::applyPendingChanges()
{
    for(std::vector<PendingChange>::iterator it = actionList.begin(); it != actionList.end(); it++)
    {
        switch((*it).action)
        {
        case Push:
            if((*it).stateID == States::ID::GAME)
                stateStack.push_back(new GameState(this, context, (*it).playerId, (*it).numberOfPlayers, (*it).socket, (*it).server));
            else
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

StateStack::PendingChange::PendingChange(Action action, States::ID stateID, int playerId, int numberOfPlayers, sf::TcpSocket *socket, GameServer *server) :
        action(action), stateID(stateID), playerId(playerId), numberOfPlayers(numberOfPlayers), socket(socket), server(server)
{
}
