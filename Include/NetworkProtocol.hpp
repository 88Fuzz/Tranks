#ifndef __NETWORKPROTOCOL_HPP__
#define __NETWORKPROTOCOL_HPP__

#define INT32(x) static_cast<sf::Int32>(x)
#define INT16(x) static_cast<sf::Int16>(x)
const unsigned short SERVER_PORT = 5000;
const unsigned short MAX_PLAYERS = 4;

namespace Server
{
    // Packets originated in the server
    enum PacketType
    {
        INITIAL_STATE,      //format: [Int32:packetType] [Int32:number of players connected]
        PLAYER_CONNECT,     //format: [Int32:packetType] [Int32:number of players connected]
        PLAYER_DISCONNECT,  //format: [Int32:packetType] [Int32:playerId disconnected] [Int32:number of players connected]
        HOST_DISCONNECT,    //format: [Int32:packetType]
        PLAYER_ACTION,      //format: [Int32:packetType] [Int32:playerIdentifier] [Int16:GUI::ButtonTypes]
        PLAYER_KILL,        //format: [Int32:packetType] [Int32:playerIdentifier doing killing] [Int32:playerIdentifier killed player] [Int16:player doing killing score]
        PLAYER_SCORE,       //format: [Int32:packetType] [Int32:playerIdentifier] [Int16:playerIdentifier score]
        START_GAME,         //format: [Int32:packetType] [Int32:numberOfPlayers] [Int32:old playerIdentifier] [Int32:new playerIdentifier]
        MESSAGE,
        GAME_OVER           //format: [Int32:packetType] [Int16:number of winners] [Int32:playerIdentifier of winner] [Int32:playerIdentifier of winner]...
    };
}

namespace Client
{
    // Packets originated in the client
    enum PacketType
    {
        //TODO these probably don't need to be 32 bits
        QUIT,       //format: [Int32:packetType]
        ACTION,     //format: [Int32:packetType] [Int32:playerIdentifier] [Int16:GUI::ButtonTypes]
        KILL,       //format: [Int32:packetType] [Int32:playerIdentifier doing killing] [Int32:playerIdentifier killed player] [Int16:player doing killing score]
        SCORE,      //format: [Int32:packetType] [Int32:playerIdentifier] [Int16:playerIdentifier score]
        START_GAME, //format: [Int32:packetType]
        CHANGE_MAX_SCORE,
        MESSAGE
    };
}

#endif
