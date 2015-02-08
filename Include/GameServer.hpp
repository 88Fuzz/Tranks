#ifndef __GAMESERVER_HPP__
#define __GAMESERVER_HPP__


#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

class GameServer
{
public:
    const unsigned short SERVER_PORT = 5000;
    GameServer();
    ~GameServer();

private:
    void handleIncomingConnections();
    void setListening(bool);
    void broadcastMessage(const std::string &);

    // A GameServerRemotePeer refers to one instance of the game, may it be local or from another computer
    struct RemoteConnection
    {
        RemoteConnection();
        sf::TcpSocket socket;
        sf::Time lastPacketTime;
        bool ready;
        bool timedOut;
    };

    //was mListeningState
    //True is server is accepting new connections
    bool acceptingConnections;

    //was mPeers
    std::vector<RemoteConnection> clientConnections;

    //was mListenerSocket
    sf::TcpListener socketListener;

    //was mConnectedPlayers
    //Used to track the next connection in clientConnections
    int connectedPlayers;
};

#endif
