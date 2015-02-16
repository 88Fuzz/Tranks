#include "State.hpp"
#include "GameState.hpp"
#include "NetworkProtocol.hpp"
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

GameState::GameState(StateStack* stack, Context context, int playerId, int numberOfPlayers, sf::TcpSocket *socket,
        GameServer *server) :
                State(stack, context),
                world(context.window, context.fonts, playerId, numberOfPlayers, socket),
                socket(socket),
                server(server)
//, player(*context.player)
{
//	player.setMissionStatus(Player::MissionRunning);
}

GameState::~GameState()
{
    delete socket;
    delete server;
}

void GameState::draw()
{
    world.draw();
}

bool GameState::handlePacket(sf::Int32 packetType, sf::Packet* packet)
{
    sf::Int32 tmp32;
    int playerDisconnected;

    switch(packetType)
    {
    //TODO ignore these messages
    case Server::INITIAL_STATE:
    case Server::PLAYER_CONNECT:
    case Server::START_GAME:
        return true;
        //TODO figure this one out
    case Server::PLAYER_DISCONNECT:
        std::cerr << "Someone left, WHAT DO I DO!?\n";
        return true;
    case Server::HOST_DISCONNECT:
        requestStackSwap(States::MAIN_MENU);
        return true;
        break;
        //ignore these messages and pass them to the world object
    case Server::PLAYER_ACTION:
    case Server::PLAYER_KILL:
    case Server::PLAYER_SCORE:
    case Server::MESSAGE:
        break;
        //TODO figure out how to handle this
    case Server::GAME_OVER:
        return true;
    }

    return false;
}

/*
 * Updates the world, if return false updates to the gameStack will stop.
 * Useful for a pause menu, will not update the background (will not make
 * a difference if pause menu is not transparent)
 */
bool GameState::update(sf::Time dt)
{
    sf::Packet packet;
    if(socket->receive(packet) == sf::Socket::Done)
    {
        //TODO add a heart beat to the server to handle timeouts
//        mTimeSinceLastPacket = sf::seconds(0.f);
        sf::Int32 packetType;
        packet >> packetType;
        if(!handlePacket(packetType, &packet))
            world.handlePacket(packetType, &packet);
    }
    else
    {
        // Check for timeout with the server
//        if(mTimeSinceLastPacket > mClientTimeout)
//        {
//            mConnected = false;
//
//            mFailedConnectionText.setString("Lost connection to server");
//            centerOrigin (mFailedConnectionText);
//
//            mFailedConnectionClock.restart();
//        }
    }

    world.update(dt);

//	if (!world.hasAlivePlayer())
//	{
//		player.setMissionStatus(Player::MissionFailure);
//		requestStackPush(States::GameOver);
//	}
//	else if (world.hasPlayerReachedEnd())
//	{
//		player.setMissionStatus(Player::MissionSuccess);
//		requestStackPush(States::GameOver);
//	}
//
//	CommandQueue& commands = world.getCommandQueue();
//	player.handleRealtimeInput(commands);

    return true;
}

/*
 * Handle SFML events, ex: mouse clicks, keyboard key presses
 */
bool GameState::handleEvent(const sf::Event* event)
{
    // Game input handling
//	CommandQueue& commands = world.getCommandQueue();
//	player.handleEvent(event, commands);

    world.handleEvent(event);

// Escape pressed, trigger the pause screen
//	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
//		requestStackPush(States::Pause);

    return true;
}
