#ifndef __LOBBYSTATE_HPP__
#define __LOBBYSTATE_HPP__

#include "State.hpp"
#include "Container.hpp"

class LobbyState: public State
{
public:
    LobbyState(StateStack *, Context, bool);

    virtual ~LobbyState();
    virtual void draw();
    virtual bool update(sf::Time);
    virtual bool handleEvent(const sf::Event *);

private:
    GUI::Container buttons;
    bool isHost;

    void loadTextures();
};

#endif
