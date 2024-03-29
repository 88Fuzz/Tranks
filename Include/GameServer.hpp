#ifndef __GAMESERVER_HPP__
#define __GAMESERVER_HPP__

#include <SFML/System/Thread.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <map>
#include <string>

class GameServer
{
public:
    const sf::Time CLIENT_TIME_OUT =  sf::seconds(3.f);
    GameServer();
    ~GameServer();

    void notifyNewConnection();

    // A GameServerRemotePeer refers to one instance of the game, may it be local or from another computer
    struct RemoteConnection
    {
        RemoteConnection();
        sf::TcpSocket socket;
        sf::Time lastPacketTime;
        sf::Int32 playerIdentifier;
        bool ready;
        bool timedOut;
    };

    // Structure to store information about current aircraft state
    struct PlayerInfo
    {
        PlayerInfo();
        sf::Int16 score;
    };

private:
    void handleIncomingConnections();
    void setListening(bool);
    void broadcastMessage(const std::string &);
    void informWorldState(RemoteConnection *);
    void executionThread();
    void sendToAll(sf::Packet*);
    void handleIncomingPackets();
    void handleIncomingPacket(sf::Packet*, RemoteConnection*, bool*);
    void handleDisconnections();
    void sendGameStartMessage();
    void tick();
    sf::Int16 incrementPlayerScore(sf::Int32, sf::Int16);
    sf::Time now() const;

    //was mListeningState
    //True is server is accepting new connections
    bool acceptingConnections;

    //was mWaitingThreadEnd
    bool stopServer;

    //was mPeers
    std::vector<RemoteConnection *> clientConnections;

    //was mListenerSocket
    sf::TcpListener socketListener;

    //was mConnectedPlayers
    //Used to track the next connection in clientConnections
    int connectedPlayers;
    sf::Clock serverClock;

    //Used to keep track of the next playerIdentifier, will not repeat if player leaves.
    //Once the game leaves lobby, these will be changed to the correct 1-4 numbers
    int playerIdentifierCount;

    //was mAircraftInfo
    std::map<sf::Int32, PlayerInfo *> playerInfoMap;

    int maxScore;

    //Server listening thread
    sf::Thread thread;
};

#endif
