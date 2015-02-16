#ifndef __GAMESTATE_HPP__
#define __GAMESTATE_HPP__

#include "State.hpp"
#include "World.hpp"
#include "NetworkProtocol.hpp"
#include "GameServer.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Network/Packet.hpp>

class GameState: public State
{
public:
    GameState(StateStack*, Context, int, int, sf::TcpSocket *, GameServer *);
    virtual ~GameState();

    virtual void draw();
    virtual bool update(sf::Time);
    virtual bool handleEvent(const sf::Event*);

private:
    bool handlePacket(sf::Int32 packetType, sf::Packet* packet);

    World world;
    sf::TcpSocket *socket;
    GameServer *server;
//    Player* player;
};

#endif
