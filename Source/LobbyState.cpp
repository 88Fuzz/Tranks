#include "LobbyState.hpp"
#include "Button.hpp"
#include "ButtonTypes.hpp"
#include "ResourceHolder.hpp"
#include "NetworkProtocol.hpp"
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <stdlib.h>
#include <fstream>
#include <sstream>

sf::IpAddress getIpFromFile()
{
    // Try to open existing file (RAII block)
    std::ifstream inputFile("ip.txt");
    std::string ipAddress;
    if(!(inputFile >> ipAddress))
        ipAddress = "127.0.0.1";

    inputFile.close();
    return ipAddress;
}

const std::string LobbyState::numPlayersStr = "Connected players: ";
LobbyState::LobbyState(StateStack *stack, Context context, bool isHost) :
                State(stack, context),
                buttons(),
                isHost(isHost),
                server(),
                socket(),
                numPlayers(0),
                numPlayersText("", context.fonts->get(Fonts::Sansation), 24)
{
    sf::IpAddress ipAddr;
    loadTextures();

    numPlayersText.setPosition(100,250);
    GUI::Button *tmpButton = new GUI::Button(context, GUI::ButtonTypes::BIG_BUTTON, 100, 400, 300, 100);
    tmpButton->setText("EXIT");
    tmpButton->setCallback([&] ()
    {
        quit();
        requestStackPop();
    });
    buttons.pack(tmpButton);

    if(isHost)
    {
        tmpButton = new GUI::Button(context, GUI::ButtonTypes::BIG_BUTTON, 100, 300, 300, 100);
        tmpButton->setText("START");
        tmpButton->setCallback([&] ()
        {
            requestStackSwap(States::GAME);
        });
        buttons.pack(tmpButton);
        ipAddr = "127.0.0.1";
    }
    else
    {
        ipAddr = getIpFromFile();
    }

    if(socket.connect(ipAddr, SERVER_PORT, sf::seconds(5.f)) == sf::TcpSocket::Done)
        socketConnected = true;
    else
        throw "Could not connect to server\n";
//        mFailedConnectionClock.restart();

    socket.setBlocking(false);

    updateNumPlayersText();
}

LobbyState::~LobbyState()
{
}

void LobbyState::quit()
{
    if(socketConnected)
    {
        // Inform server this client is dying
        sf::Packet packet;
        packet << static_cast<sf::Int32>(Client::QUIT);
        socket.send(packet);
    }
}

void LobbyState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.setView(window.getDefaultView());

    window.draw(buttons);
    window.draw(numPlayersText);
}

/*
 * Updates the world, if return false updates to the gameStack will stop.
 * Useful for a pause menu, will not update the background (will not make
 * a difference if pause menu is not transparent)
 */
bool LobbyState::update(sf::Time dt)
{
    // Handle messages from server that may have arrived
    sf::Packet packet;
    if(socket.receive(packet) == sf::Socket::Done)
    {
        //TODO add a heart beat to the server to handle timeouts
//        mTimeSinceLastPacket = sf::seconds(0.f);
        sf::Int32 packetType;
        packet >> packetType;
        handlePacket(packetType, &packet);
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
    return true;
}

void LobbyState::handlePacket(sf::Int32 packetType, sf::Packet* packet)
{
    std::cout << "HANDLING PACKET " << packetType << "\n";
    sf::Int32 tmp32;

    switch(packetType)
    {
    case Server::INITIAL_STATE:
    case Server::PLAYER_CONNECT:
        *packet >> tmp32;
        numPlayers = tmp32;
        LobbyState::updateNumPlayersText();
        break;
        //TODO figure this one out
    case Server::PLAYER_DISCONNECT:
        *packet >> tmp32;
        numPlayers = tmp32;
        LobbyState::updateNumPlayersText();
        std::cout << "Player disconnect\n";
        break;
    case Server::HOST_DISCONNECT:
        std::cout << "SERVER DISCONNECT\n";
            requestStackSwap(States::MAIN_MENU);
        break;
        //TODO ignore message in the lobby, shouldn't happen
    case Server::PLAYER_ACTION:
    case Server::PLAYER_KILL:
    case Server::PLAYER_SCORE:
    case Server::GAME_OVER:
        std::cerr << "Why am I getting this message " << packetType << ", shouldn't be here\n";
        break;
    case Server::MESSAGE:
        break;
    }
}

/*
 * Handle SFML events, ex: mouse clicks, keyboard key presses
 */
bool LobbyState::handleEvent(const sf::Event *event)
{
    GUI::Component* selected;
    if(event->type == sf::Event::MouseButtonPressed)
    {
        buttons.handleEvent(event);
    }
    else if(event->type == sf::Event::MouseButtonReleased)
    {
        if((selected = buttons.getSelectedComponent()) != NULL)
            ((GUI::Button*) selected)->trigger();
    }

    return true;
}

void LobbyState::loadTextures()
{
    TextureHolder* textures = getContext().textures;
    textures->load(Textures::BUTTONS, "Media/Textures/Buttons.png");
}

void LobbyState::updateNumPlayersText()
{
    std::stringstream ss;
    std::cout << "\t\t changing number of players " << numPlayers << "\n";
    ss << numPlayersStr << numPlayers;
    numPlayersText.setString(ss.str());
}
