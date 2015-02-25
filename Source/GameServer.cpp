#include "GameServer.hpp"
#include "NetworkProtocol.hpp"
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <iostream>

GameServer::RemoteConnection::RemoteConnection() :
        ready(false), timedOut(false), playerIdentifier(-1), socket(), lastPacketTime()
{
    socket.setBlocking(false);
}

GameServer::PlayerInfo::PlayerInfo() :
        score(0)
{

}

GameServer::GameServer() :
                thread(&GameServer::executionThread, this),
                acceptingConnections(false),
                stopServer(false),
                connectedPlayers(0),
                serverClock(),
                playerInfoMap(),
                maxScore(5),
                playerIdentifierCount(0)
{
    socketListener.setBlocking(false);
    clientConnections.push_back(new RemoteConnection()), thread.launch();
}

GameServer::~GameServer()
{
    stopServer = true;
    std::vector<RemoteConnection *> clientConnections;
    for(std::vector<RemoteConnection *>::iterator it = clientConnections.begin(); it != clientConnections.end(); it++)
    {
        delete (*it);
    }

    for(std::map<sf::Int32, PlayerInfo*>::iterator itr = playerInfoMap.begin(); itr != playerInfoMap.end(); itr++)
    {
        delete (itr->second);
    }
}

/*
 * Setter for accpectingConnections.
 */
void GameServer::setListening(bool enable)
{
    // Check if it isn't already listening
    if(enable)
    {
        if(!acceptingConnections)
            acceptingConnections = (socketListener.listen(SERVER_PORT) == sf::TcpListener::Done);
    }
    else
    {
        socketListener.close();
        acceptingConnections = false;
    }
}

/*
 * Main server thread
 */
void GameServer::executionThread()
{
    setListening(true);

    sf::Time stepInterval = sf::seconds(1.f / 60.f);
    sf::Time stepTime = sf::Time::Zero;
    sf::Time tickInterval = sf::seconds(1.f / 20.f);
    sf::Time tickTime = sf::Time::Zero;
    sf::Clock stepClock, tickClock;

    while(!stopServer)
    {
        handleIncomingPackets();
        handleIncomingConnections();

        stepTime += stepClock.getElapsedTime();
        stepClock.restart();

        tickTime += tickClock.getElapsedTime();
        tickClock.restart();

        // Fixed tick step
        while(tickTime >= tickInterval)
        {
            tick();
            tickTime -= tickInterval;
        }

        // Sleep to prevent server from consuming 100% CPU
        sf::sleep(sf::milliseconds(100));
    }
}

/*
 * Check if there is an incoming connection
 */
void GameServer::handleIncomingConnections()
{
    if(!acceptingConnections)
        return;

    if(socketListener.accept(clientConnections[connectedPlayers]->socket) == sf::TcpListener::Done)
    {
        sf::Packet packet;

        informWorldState(clientConnections[connectedPlayers]);
        notifyNewConnection();

        clientConnections[connectedPlayers]->socket.send(packet);
        clientConnections[connectedPlayers]->ready = true;
        clientConnections[connectedPlayers]->lastPacketTime = now();
        clientConnections[connectedPlayers]->playerIdentifier = ++playerIdentifierCount;
        connectedPlayers++;

        clientConnections.push_back(new RemoteConnection());
        if(connectedPlayers >= MAX_PLAYERS)
            setListening(false);
    }
}

void GameServer::handleIncomingPackets()
{
    bool detectedTimeout = false;
    sf::Packet packet;

    for(std::vector<RemoteConnection *>::iterator it = clientConnections.begin(); it != clientConnections.end(); ++it)
    {
        if((*it)->ready)
        {
            //Loop through all packets of clients
            while((*it)->socket.receive(packet) == sf::Socket::Done)
            {
                // Interpret packet and react to it
                handleIncomingPacket(&packet, (*it), &detectedTimeout);

                // Packet was indeed received, update the ping timer
                (*it)->lastPacketTime = now();
                packet.clear();
            }

            //TODO implement a heartbeat
            /*
            if(now() >= (*it)->lastPacketTime + CLIENT_TIME_OUT)
            {
                (*it)->timedOut = true;
                detectedTimeout = true;
            }
            */
        }
    }

    if(detectedTimeout)
        handleDisconnections();
}

void GameServer::sendGameStartMessage()
{
    sf::Packet packet;
    int j = 0;

    packet << INT32(Server::START_GAME);
    packet << INT32(clientConnections.size() - 1);
    for(std::vector<RemoteConnection *>::iterator it = clientConnections.begin(); it != clientConnections.end(); it++)
    {
        if((*it)->ready)
        {
            packet << INT32((*it)->playerIdentifier) << INT32(j);
            playerInfoMap.insert(std::pair<sf::Int32, PlayerInfo *>(INT32(j), new PlayerInfo()));
            (*it)->playerIdentifier = j++;
        }
    }

    sendToAll(&packet);
}

void GameServer::handleDisconnections()
{
    for(auto itr = clientConnections.begin(); itr != clientConnections.end();)
    {
        if((*itr)->timedOut)
        {
            // Inform everyone of the disconnection, erase
            sf::Int32 identifier = (*itr)->playerIdentifier;

            playerInfoMap.erase(identifier);

            connectedPlayers--;

            itr = clientConnections.erase(itr);

            // Go back to a listening state if needed
            if(connectedPlayers < MAX_PLAYERS)
            {
                clientConnections.push_back(new RemoteConnection());
                setListening(true);
            }
        }
        else
        {
            ++itr;
        }
    }
}

void GameServer::handleIncomingPacket(sf::Packet* packet, RemoteConnection* connection, bool* detectedTimeout)
{
    sf::Int32 packetType;
    sf::Int32 playerId;
    sf::Int16 action;
    sf::Int32 killer;
    sf::Int32 killed;
    sf::Int16 playerScore;
    sf::Int16 score;

    *packet >> packetType;

    //TODO add playerId validation, and all validation checks on client packets
    switch(packetType)
    {
    case Client::QUIT:
        //Player is host
        if(connection->playerIdentifier == 0)
        {
            stopServer = true;
            sendToAll(&(sf::Packet() << INT32(Server::HOST_DISCONNECT)));
        }
        else
            sendToAll(&(sf::Packet() << INT32(Server::PLAYER_DISCONNECT) << INT32(connection->playerIdentifier)
                            << INT32(connectedPlayers - 1)));

        connection->timedOut = true;
        *detectedTimeout = true;
        break;
    case Client::ACTION:
        *packet >> playerId;
        *packet >> action;
        sendToAll(&(sf::Packet() << INT32(Server::PLAYER_ACTION) << INT32(playerId) << INT16(action)));
        break;
    case Client::KILL:
        *packet >> killer;
        *packet >> killed;
        sendToAll(&(sf::Packet() << INT32(Server::PLAYER_KILL) << killer << killed));
        break;
    case Client::SCORE:
        *packet >> playerId;
        *packet >> playerScore;
        score = incrementPlayerScore(playerId, playerScore);
        sendToAll(&(sf::Packet() << INT32(Server::PLAYER_SCORE) << playerId << score));
        break;
    case Client::MESSAGE:
        break;
    case Client::START_GAME:
        sendGameStartMessage();
        setListening(false);
        break;
    }
}

sf::Time GameServer::now() const
{
    return serverClock.getElapsedTime();
}

/*
 * Used to supply new connections with any important information about the session
 */
void GameServer::informWorldState(RemoteConnection *connection)
{
    sf::Packet packet;
    packet << INT32(Server::INITIAL_STATE);
    packet << INT32(connectedPlayers + 1);
    connection->socket.send(packet);
}

/*
 * DO NOT USE YET, used to send messages to all clients, but not finished
 */
void GameServer::broadcastMessage(const std::string& message)
{
    for(std::size_t i = 0; i < connectedPlayers; ++i)
    {
        if(clientConnections[i]->ready)
        {
            sf::Packet packet;
            packet << INT32(Server::MESSAGE);
            packet << message;

            clientConnections[i]->socket.send(packet);
        }
    }
}

void GameServer::notifyNewConnection()
{
    sf::Packet packet;
    packet << INT32(Server::PLAYER_CONNECT) << INT32(connectedPlayers + 1);

    for(std::size_t i = 0; i < connectedPlayers; ++i)
    {
        if(clientConnections[i]->ready)
        {
            clientConnections[i]->socket.send(packet);
        }
    }
}

void GameServer::sendToAll(sf::Packet* packet)
{
    for(std::vector<RemoteConnection *>::iterator it = clientConnections.begin(); it != clientConnections.end(); ++it)
    {
        if((*it)->ready)
            (*it)->socket.send(*packet);
    }
}

sf::Int16 GameServer::incrementPlayerScore(sf::Int32 player, sf::Int16 validation)
{
    sf::Int16 score = ++(playerInfoMap[player]->score);
    if(validation != score)
    {
        std::cerr << "We have mismatching scores player thinks : " << validation << " server thinks " << score
                << ". Possible cheater, or game and server are out of sync\n";
    }
    return score;
}

void GameServer::tick()
{
    std::vector<sf::Int32> winners;
    sf::Packet missionSuccessPacket;

    //Check if player has won game
    for(std::map<sf::Int32, PlayerInfo*>::iterator it = playerInfoMap.begin(); it != playerInfoMap.end(); it++)
    {
        if(it->second->score >= maxScore)
        {
            winners.push_back(it->first);
        }
    }

    if(winners.size() > 1)
    {
        missionSuccessPacket << INT32(Server::GAME_OVER) << INT16(winners.size());
        for(std::vector<sf::Int32>::iterator it = winners.begin(); it != winners.end(); it++)
        {
            missionSuccessPacket << INT32(*it) << INT32(winners.size());
        }
        sendToAll(&missionSuccessPacket);
    }
}
