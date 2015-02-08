#include "GameServer.hpp"

GameServer::GameServer() :
        acceptingConnections(false)
{

}
GameServer::~GameServer()
{
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

void GameServer::handleIncomingConnections()
{
    if(!acceptingConnections)
        return;

    if(socketListener.accept(clientConnections[connectedPlayers].socket) == sf::TcpListener::Done)
    {
        //order the new client to spawn its own plane ( player 1 )
//        mAircraftInfo[mAircraftIdentifierCounter].position = sf::Vector2f(mBattleFieldRect.width / 2,
//                mBattleFieldRect.top + mBattleFieldRect.height / 2);
//        mAircraftInfo[mAircraftIdentifierCounter].hitpoints = 100;
//        mAircraftInfo[mAircraftIdentifierCounter].missileAmmo = 2;

        sf::Packet packet;
//        packet << static_cast<sf::Int32>(Server::SpawnSelf);
//        packet << mAircraftIdentifierCounter;
//        packet << mAircraftInfo[mAircraftIdentifierCounter].position.x;
//        packet << mAircraftInfo[mAircraftIdentifierCounter].position.y;
//
//        clientConnections[connectedPlayers]->aircraftIdentifiers.push_back(mAircraftIdentifierCounter);

        broadcastMessage("New player!");
        //TODO Don't think I need this function
        informWorldState(clientConnections[connectedPlayers].socket);
        //TODO don't think I need this function either
        notifyPlayerSpawn(mAircraftIdentifierCounter++);

        clientConnections[connectedPlayers].socket.send(packet);
        clientConnections[connectedPlayers].ready = true;
        clientConnections[connectedPlayers].lastPacketTime = now(); // prevent initial timeouts
//        mAircraftCount++;
        connectedPlayers++;

        if(connectedPlayers >= mMaxConnectedPlayers)
            setListening(false);
        else
            // Add a new waiting peer
            clientConnections.push_back(RemoteConnection());
    }
}

void GameServer::broadcastMessage(const std::string& message)
{
    for(std::size_t i = 0; i < connectedPlayers; ++i)
    {
        if(clientConnections[i].ready)
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32>(Server::BroadcastMessage);
            packet << message;

            clientConnections[i].socket.send(packet);
        }
    }
}
