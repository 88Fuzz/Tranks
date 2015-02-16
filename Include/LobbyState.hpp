#ifndef __LOBBYSTATE_HPP__
#define __LOBBYSTATE_HPP__

#include "State.hpp"
#include "Container.hpp"
#include "GameServer.hpp"
#include <SFML/Graphics/Text.hpp>

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
    void handlePacket(sf::Int32, sf::Packet*);
    void loadTextures();
    void updateNumPlayersText();
    void sendStartGameMessage();

    static const std::string numPlayersStr;
    GUI::Container buttons;
    sf::Text numPlayersText;
    GameServer* server;
    sf::TcpSocket* socket;
    bool isHost;
    bool socketConnected;
    sf::Int16 numPlayers;
    int playerId;
};

#endif
