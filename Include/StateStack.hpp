#ifndef __STATESTACK_HPP__
#define __STATESTACK_HPP__

#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"
#include "GameServer.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Network/Packet.hpp>
#include <vector>
#include <utility>
#include <functional>
#include <map>

namespace sf
{
    class Event;
    class RenderWindow;
}

//class State;

class StateStack: private sf::NonCopyable
{
public:
    enum Action
    {
        Push,
        Pop,
        Clear,
    };

    explicit StateStack(State::Context);

    template<typename T> void registerState(States::ID);
    template<typename T, typename Param1> void registerState(States::ID, Param1);

    void update(sf::Time);
    void draw();
    void handleEvent(const sf::Event*);

    void swapState(States::ID);
    void swapState(States::ID, int, int, sf::TcpSocket*, GameServer *);
    void pushState(States::ID);
    void popState();
    void clearStates();

    bool isEmpty() const;

private:
    struct PendingChange
    {
        //TODO this should be done MUCH better. Pass in a generic map that can be used for any state
        explicit PendingChange(Action action, States::ID stateID = States::NONE, int playerId = 0, int numberOfPlayers =
                0, sf::TcpSocket *socket = NULL, GameServer *server = NULL);

        Action action;
        States::ID stateID;
        int playerId;
        int numberOfPlayers;
        sf::TcpSocket *socket;
        GameServer *server;
    };

    std::vector<State *> stateStack;
    std::vector<PendingChange> actionList;

    State::Context context;
    std::map<States::ID, std::function<State*()>> factories;
    State* createState(States::ID);
    void destroyState(State *);
    void applyPendingChanges();
};

/*
 * Pushing the state into a special list of commands.
 * It could be unsafe to change the actual state vector (due to memory allocations)
 * so actual modifications are waited until it is known to be safe.
 */
template<typename T>
void StateStack::registerState(States::ID stateID)
{
    factories[stateID] = [this] ()
    {
        return (new T(this, context));
    };
}

template<typename T, typename Param1>
void StateStack::registerState(States::ID stateID, Param1 arg1)
{
    factories[stateID] = [this, arg1] ()
    {
        return (new T(this, context, arg1));
    };
}

//template <typename T, typename Param1>
//void StateStack::registerState(States::ID stateID, Param1 arg1)
//{
//    factories[stateID] = [this, arg1] ()
//    {
//        return (new T(*this, context, arg1));
//    };
//}

#endif
