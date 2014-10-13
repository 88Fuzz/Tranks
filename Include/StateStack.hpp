#ifndef __STATESTACK_HPP__
#define __STATESTACK_HPP__

#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
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

    template<typename T>
    void registerState(States::ID);

    void update(sf::Time);
    void draw();
    void handleEvent(const sf::Event*);

    void pushState(States::ID);
    void popState();
    void clearStates();

    bool isEmpty() const;

private:
    struct PendingChange
    {
        explicit PendingChange(Action action, States::ID stateID = States::None);

        Action action;
        States::ID stateID;
    };

    std::vector<State *> stateStack;
    std::vector<PendingChange> actionList;

    State::Context context;
    std::map<States::ID, std::function<State*()>> factories;
    State* createState(States::ID);
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

#endif
