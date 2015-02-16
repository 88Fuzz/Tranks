#ifndef __STATE_HPP__
#define __STATE_HPP__

#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"
#include "GameServer.hpp"
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

class StateStack;
namespace sf
{
    class RenderWindow;
}

class State
{
public:

    struct Context
    {
        Context(sf::RenderWindow* window, TextureHolder* textures, FontHolder* fonts);

        sf::RenderWindow* window;
        TextureHolder* textures;
        FontHolder* fonts;
    };

    State(StateStack*, Context);
    virtual ~State();

    virtual void draw() = 0;
    virtual bool update(sf::Time) = 0;
    virtual bool handleEvent(const sf::Event*) = 0;

protected:
    //These requests are used to change states. For example, if game needs to go from
    //main menu to the game, use these
    void requestStackSwap(States::ID);
    void requestStackSwap(States::ID, int, int, sf::TcpSocket*, GameServer*);
    void requestStackPush(States::ID);
    void requestStackPop();
    void requestStateClear();

    Context getContext() const;

private:
    StateStack* stack;
    Context context;
};

#endif
