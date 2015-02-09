#ifndef __NETWORKPROTOCOL_HPP__
#define __NETWORKPROTOCOL_HPP__

const unsigned short SERVER_PORT = 5000;
const unsigned short MAX_PLAYERS = 16;

namespace Server
{
    // Packets originated in the server
    enum PacketType
    {
        INITIAL_STATE,
        PLAYER_CONNECT,     //format: [Int32:packetType] [Int16:number of players connected]
        PLAYER_DISCONNECT,
        PLAYER_ACTION,      //format: [Int32:packetType] [Int32:playerIdentifier] [Int16:GUI::ButtonTypes]
        PLAYER_KILL,        //format: [Int32:packetType] [Int32:playerIdentifier doing killing] [Int32:playerIdentifier killed player] [Int16:player doing killing score]
        PLAYER_SCORE,       //format: [Int32:packetType] [Int32:playerIdentifier] [Int16:playerIdentifier score]
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
        CHANGE_MAX_SCORE,
        MESSAGE
    };
}

#endif
