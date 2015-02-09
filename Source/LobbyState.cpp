#include "LobbyState.hpp"
#include "Button.hpp"
#include "ButtonTypes.hpp"
#include "ResourceHolder.hpp"
#include "NetworkProtocol.hpp"
#include <stdlib.h>

sf::IpAddress getIpFromFile()
{
    { // Try to open existing file (RAII block)
        std::ifstream inputFile("ip.txt");
        std::string ipAddress;
        if(inputFile >> ipAddress)
            return ipAddress;
    }

    // If open/read failed, create new file
    std::ofstream outputFile("ip.txt");
    std::string localAddress = "127.0.0.1";
    outputFile << localAddress;
    return localAddress;
}

LobbyState::LobbyState(StateStack *stack, Context context, bool isHost) :
        State(stack, context), buttons(), isHost(isHost), server(), socket()
{
    sf::IpAddress ipAddr;
    loadTextures();

    GUI::Button *tmpButton = new GUI::Button(context, GUI::ButtonTypes::BIG_BUTTON, 100, 400, 300, 100);
    tmpButton->setText("EXIT");
    tmpButton->setCallback([&] ()
    {
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

}

LobbyState::~LobbyState()
{
}

void LobbyState::quit()
{
    if(!isHost && socketConnected)
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
        handlePacket(packetType, packet);
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

void MultiplayerGameState::handlePacket(sf::Int32 packetType, sf::Packet& packet)
{
    switch(packetType)
    {
    // Send message to all clients
    case Server::BroadcastMessage:
        std::string message;
        packet >> message;
        mBroadcasts.push_back(message);

        // Just added first message, display immediately
        if(mBroadcasts.size() == 1)
        {
            mBroadcastText.setString(mBroadcasts.front());
            centerOrigin (mBroadcastText);
            mBroadcastElapsedTime = sf::Time::Zero;
        }
        break;

        // Sent by the server to order to spawn player 1 airplane on connect
    case Server::SpawnSelf:
        sf::Int32 aircraftIdentifier;
        sf::Vector2f aircraftPosition;
        packet >> aircraftIdentifier >> aircraftPosition.x >> aircraftPosition.y;

        Aircraft* aircraft = mWorld.addAircraft(aircraftIdentifier);
        aircraft->setPosition(aircraftPosition);

        mPlayers[aircraftIdentifier].reset(new Player(&mSocket, aircraftIdentifier, getContext().keys1));
        mLocalPlayerIdentifiers.push_back(aircraftIdentifier);

        mGameStarted = true;
        break;

        //
    case Server::PlayerConnect:
        sf::Int32 aircraftIdentifier;
        sf::Vector2f aircraftPosition;
        packet >> aircraftIdentifier >> aircraftPosition.x >> aircraftPosition.y;

        Aircraft* aircraft = mWorld.addAircraft(aircraftIdentifier);
        aircraft->setPosition(aircraftPosition);

        mPlayers[aircraftIdentifier].reset(new Player(&mSocket, aircraftIdentifier, nullptr));
        break;
        //
    case Server::PlayerDisconnect:
        sf::Int32 aircraftIdentifier;
        packet >> aircraftIdentifier;

        mWorld.removeAircraft(aircraftIdentifier);
        mPlayers.erase(aircraftIdentifier);
        break;

        //
    case Server::InitialState:
        sf::Int32 aircraftCount;
        float worldHeight, currentScroll;
        packet >> worldHeight >> currentScroll;

        mWorld.setWorldHeight(worldHeight);
        mWorld.setCurrentBattleFieldPosition(currentScroll);

        packet >> aircraftCount;
        for(sf::Int32 i = 0; i < aircraftCount; ++i)
        {
            sf::Int32 aircraftIdentifier;
            sf::Int32 hitpoints;
            sf::Int32 missileAmmo;
            sf::Vector2f aircraftPosition;
            packet >> aircraftIdentifier >> aircraftPosition.x >> aircraftPosition.y >> hitpoints >> missileAmmo;

            Aircraft* aircraft = mWorld.addAircraft(aircraftIdentifier);
            aircraft->setPosition(aircraftPosition);
            aircraft->setHitpoints(hitpoints);
            aircraft->setMissileAmmo(missileAmmo);

            mPlayers[aircraftIdentifier].reset(new Player(&mSocket, aircraftIdentifier, nullptr));
        }
        break;
        //
    case Server::AcceptCoopPartner:
        sf::Int32 aircraftIdentifier;
        packet >> aircraftIdentifier;

        mWorld.addAircraft(aircraftIdentifier);
        mPlayers[aircraftIdentifier].reset(new Player(&mSocket, aircraftIdentifier, getContext().keys2));
        mLocalPlayerIdentifiers.push_back(aircraftIdentifier);
        break;

        // Player event (like missile fired) occurs
    case Server::PlayerEvent:
        sf::Int32 aircraftIdentifier;
        sf::Int32 action;
        packet >> aircraftIdentifier >> action;

        auto itr = mPlayers.find(aircraftIdentifier);
        if(itr != mPlayers.end())
            itr->second->handleNetworkEvent(static_cast<Player::Action>(action), mWorld.getCommandQueue());
        break;

        // Player's movement or fire keyboard state changes
    case Server::PlayerRealtimeChange:
        sf::Int32 aircraftIdentifier;
        sf::Int32 action;
        bool actionEnabled;
        packet >> aircraftIdentifier >> action >> actionEnabled;

        auto itr = mPlayers.find(aircraftIdentifier);
        if(itr != mPlayers.end())
            itr->second->handleNetworkRealtimeChange(static_cast<Player::Action>(action), actionEnabled);
        break;

        // New enemy to be created
    case Server::SpawnEnemy:
        float height;
        sf::Int32 type;
        float relativeX;
        packet >> type >> height >> relativeX;

        mWorld.addEnemy(static_cast<Aircraft::Type>(type), relativeX, height);
        mWorld.sortEnemies();
        break;
        // Mission successfully completed
    case Server::MissionSuccess:
        requestStackPush(States::MissionSuccess);
        break;

        // Pickup created
    case Server::SpawnPickup:
        sf::Int32 type;
        sf::Vector2f position;
        packet >> type >> position.x >> position.y;

        mWorld.createPickup(position, static_cast<Pickup::Type>(type));
        break;

        //
    case Server::UpdateClientState:
        float currentWorldPosition;
        sf::Int32 aircraftCount;
        packet >> currentWorldPosition >> aircraftCount;

        float currentViewPosition = mWorld.getViewBounds().top + mWorld.getViewBounds().height;

        // Set the world's scroll compensation according to whether the view is behind or too advanced
        mWorld.setWorldScrollCompensation(currentViewPosition / currentWorldPosition);

        for(sf::Int32 i = 0; i < aircraftCount; ++i)
        {
            sf::Vector2f aircraftPosition;
            sf::Int32 aircraftIdentifier;
            packet >> aircraftIdentifier >> aircraftPosition.x >> aircraftPosition.y;

            Aircraft* aircraft = mWorld.getAircraft(aircraftIdentifier);
            bool isLocalPlane = std::find(mLocalPlayerIdentifiers.begin(), mLocalPlayerIdentifiers.end(),
                    aircraftIdentifier) != mLocalPlayerIdentifiers.end();
            if(aircraft && !isLocalPlane)
            {
                sf::Vector2f interpolatedPosition = aircraft->getPosition()
                        + (aircraftPosition - aircraft->getPosition()) * 0.1f;
                aircraft->setPosition(interpolatedPosition);
            }
        }
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
