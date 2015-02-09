#ifndef __LOBBYSTATE_HPP__
#define __LOBBYSTATE_HPP__

#include "State.hpp"
#include "Container.hpp"
#include "GameServer.hpp"

class LobbyState: public State
{
public:
    LobbyState(StateStack *, Context, bool);

    virtual ~LobbyState();
    virtual void draw();
    virtual bool update(sf::Time);
    virtual bool handleEvent(const sf::Event *);
    void quit();

private:
    GUI::Container buttons;
    GameServer server;
    sf::TcpSocket socket;
    bool isHost;
    bool socketConnected;

    void loadTextures();
};

#endif
