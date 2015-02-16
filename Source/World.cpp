#include "World.hpp"
#include "MapCreator.hpp"
#include "Bullet.hpp"
#include "Button.hpp"
#include "ButtonTypes.hpp"
#include "NetworkProtocol.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <algorithm>
#include <stdlib.h>
#include <cmath>
#include <limits>
#include <iostream>

//TODO clean up this code

World::World(sf::RenderWindow* outputTarget, FontHolder* fonts, int playerId, int numberOfPlayers,
        sf::TcpSocket *socket) :
                window(outputTarget),
                sceneTexture(),
                worldView(outputTarget->getDefaultView()),
                textures(),
                fonts(fonts),
                sceneGraph(),
                sceneLayers(),
                worldBounds(0.f, 0.f, worldView.getSize().x, worldView.getSize().y),
                map(NULL),
                trankControls(),
                sendCommandBox(),
                playersReady(0),
                currState(IDLE),
                tileSize(0),
                numPlayers(numberOfPlayers),
                playerId(playerId),
                socket(socket)
{
    sceneTexture.create(window->getSize().x, window->getSize().y);

    loadTextures();

    for(int j = 0; j < numPlayers; j++)
    {
        players.push_back(new Player(j, &textures));
        pendingPlayerCommands.push_back(GUI::ButtonTypes::CHECK_BOX);
    }

    buildScene();
}

World::~World()
{
    if(map != NULL)
        delete map;

    for(int j = 0; j < numPlayers; j++)
        delete players[j];
}

void World::handlePacket(sf::Int32 packetType, sf::Packet* packet)
{
    sf::Int32 tmp32;
    sf::Int16 tmp16;
    int player;
    int command;

    switch(packetType)
    {
    case Server::PLAYER_DISCONNECT:
        std::cerr << "Someone left, WHAT DO I DO!?\n";
        break;
    case Server::PLAYER_ACTION:
    case Server::PLAYER_KILL:
    case Server::PLAYER_SCORE:
        *packet >> tmp32;
        player = tmp32;
        *packet >> tmp16;
        command = tmp16;
        if(player < numPlayers && tmp32 < GUI::ButtonTypes::BUTTON_TYPES_COUNT)
        {
            playersReady++;
            pendingPlayerCommands[player] = static_cast<GUI::ButtonTypes>(command);
        }
        else
            std::cerr << "Action from player " << player << " is greater than " << numPlayers
                    << " or not a valid action " << command << "\n";

        break;
    case Server::GAME_OVER:
        std::cerr << "Why am I getting this message " << packetType << ", shouldn't be here\n";
        break;
        //TODO figure out how to handle these
    case Server::MESSAGE:
        break;
    }
}

void World::update(sf::Time dt)
{
    Command *command;
    int completed = 0;
    // Scroll the world, reset player velocity
//	worldView.move(0.f, mScrollSpeed * dt.asSeconds());

// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
//	while (!mCommandQueue.isEmpty())
//		sceneGraph.onCommand(mCommandQueue.pop(), dt);
//    adaptPlayerVelocity();

// Collision detection and response (may destroy entities)
//	handleCollisions();

// Remove all destroyed entities, create new ones
//	sceneGraph.removeWrecks();

//TODO un-comment this statement
//    if(players.size() == pendingPlayerCommands.size())

    if(currState == IDLE)
    {
        if(playersReady == numPlayers)
        {
            validateMoves();
            queueMovementActions();
            playersReady = 0;
            currState = MOVE_ACTIONS;
        }
    }

    //TODO figure out how to do this
    while(!commandQueue.isEmpty())
    {
        command = commandQueue.pop();
        command->action();
        delete command;
    }

    //Check if players are finished executing actions
    if(currState == MOVE_ACTIONS || currState == SHOOT_ACTIONS)
    {
        for(int j = 0; j < numPlayers; j++)
        {
            if(!players[j]->isActionExecuting() || !players[j]->isAlive())
            {
                completed++;
            }
        }

        if(completed == numPlayers)
        {
            if(currState == MOVE_ACTIONS)
            {
                currState = SHOOT_ACTIONS;
                queueShootActions();
            }
            else
                currState = END_ACTIONS;
        }
    }

    if(currState == END_ACTIONS)
    {
        for(int j = 0; j < numPlayers; j++)
        {
            if(!players[j]->isAlive())
                players[j]->incrementDeathClock();
        }
        trankControls.deselect();
        sendCommandBox.deselect();

        currState = IDLE;
    }
    // Regular update step, adapt position (correct if outside view)
    sceneGraph.update(dt);
}

void World::draw()
{
    window->setView(worldView);
    window->draw(sceneGraph);
    window->draw(trankControls);
    window->draw(sendCommandBox);
}

void World::queueShootActions()
{
    Command *command;
    for(int j = 0; j < numPlayers; j++)
    {
        if(players[j]->isAlive())
        {
            if(pendingPlayerCommands[j] == GUI::ButtonTypes::FIRE)
            {
                command = new Command();
                command->category = Category::Type::NONE;
                command->action = [=]()
                {
                    players[j]->startFire();
                };
                commandQueue.push(command);
            }
        }
    }
}

/*
 * Adds all movements from players to the command queue
 */
void World::queueMovementActions()
{
    Command *command;
    for(int j = 0; j < numPlayers; j++)
    {
        if(players[j]->isAlive())
        {
            switch(pendingPlayerCommands[j])
            {
            case GUI::ButtonTypes::MOVE_SINGLE:
                command = new Command();
                command->category = Category::Type::NONE;
                command->action = [=]()
                {
                    sf::Vector2i tilePos = players[j]->getTilePos();

                    //Remove player from its parent node
                        if(map->removePlayerChildNode(MapCreator::get1d(tilePos.x, tilePos.y, mapTileWidth))==NULL)
                        std::cerr << "Shit be horribly broken\n";

                        //get player's new tilePos, and attach it to the new parent node
                        tilePos = players[j]->getTilePos(1);

                        map->layerChildNode(players[j],MapCreator::get1d(tilePos.x, tilePos.y, mapTileWidth));
                        players[j]->startMovement(Player::SINGLE_MOVE);
                    };
                commandQueue.push(command);
                break;
            case GUI::ButtonTypes::MOVE_DOUBLE:
                command = new Command();
                command->category = Category::Type::NONE;
                command->action = [=]()
                {
                    sf::Vector2i tilePos = players[j]->getTilePos();

                    //Remove player from its parent node
                        if(map->removePlayerChildNode(MapCreator::get1d(tilePos.x, tilePos.y, mapTileWidth))==NULL)
                        std::cerr << "Shit be horribly broken\n";

                        tilePos = players[j]->getTilePos(2);

                        map->layerChildNode(players[j],MapCreator::get1d(tilePos.x, tilePos.y, mapTileWidth));
                        players[j]->startMovement(Player::DOUBLE_MOVE);
                    };
                commandQueue.push(command);
                break;
            case GUI::ButtonTypes::ROTATE_HALF_CLOCKWISE:
                command = new Command();
                command->category = Category::Type::NONE;
                command->action = [=]()
                {
                    players[j]->startRotation(Player::CLOCKWISE,Player::SINGLE_ROTATION);
                };
                commandQueue.push(command);
                break;
            case GUI::ButtonTypes::ROTATE_HALF_COUNTER:
                command = new Command();
                command->category = Category::Type::NONE;
                command->action = [=]()
                {
                    players[j]->startRotation(Player::COUNTER_CLOCKWISE,Player::SINGLE_ROTATION);
                };
                commandQueue.push(command);
                break;
            case GUI::ButtonTypes::ROTATE_FULL:
                command = new Command();
                command->category = Category::Type::NONE;
                command->action = [=]()
                {
                    players[j]->startRotation(Player::CLOCKWISE,Player::DOUBLE_ROTATION);
                };
                commandQueue.push(command);
                break;
            }
        }
    }
}

void World::validateMoves()
{
    Moveable::Direction facing;
    sf::Vector2i tilePos;
    sf::Vector2i pendingLocations[pendingPlayerCommands.size()];

    for(int j = 0; j < numPlayers; j++)
    {
        pendingLocations[j] = players[j]->getTilePos();
        switch(pendingPlayerCommands[j])
        {
        case GUI::ButtonTypes::MOVE_SINGLE:
            if(validateAction(players[j], 1))
            {
                pendingLocations[j] = players[j]->getTilePos(1);
            }
            else
            {
                pendingPlayerCommands[j] = GUI::ButtonTypes::CHECK_BOX;
            }
            break;
        case GUI::ButtonTypes::MOVE_DOUBLE:
            if(validateAction(players[j], 2))
            {
                pendingLocations[j] = players[j]->getTilePos(2);
            }
            else
            {
                pendingPlayerCommands[j] = GUI::ButtonTypes::CHECK_BOX;
            }
            break;
        }
    }

    /*
     * Check if future movents will cause colisions with other Tranks, if so remove their movements from pengingCommands
     */
    for(int j = 0; j < pendingPlayerCommands.size(); j++)
    {
        for(int i = j + 1; i < pendingPlayerCommands.size(); i++)
        {
            if(pendingLocations[j].x == pendingLocations[i].x && pendingLocations[j].y == pendingLocations[i].y)
            {
                if(pendingPlayerCommands[j] == GUI::ButtonTypes::MOVE_SINGLE
                        || pendingPlayerCommands[j] == GUI::ButtonTypes::MOVE_DOUBLE)
                {
                    pendingPlayerCommands[j] = GUI::ButtonTypes::CHECK_BOX;
                }

                if(pendingPlayerCommands[i] == GUI::ButtonTypes::MOVE_SINGLE
                        || pendingPlayerCommands[i] == GUI::ButtonTypes::MOVE_DOUBLE)
                {
                    pendingPlayerCommands[i] = GUI::ButtonTypes::CHECK_BOX;
                }
            }
        }
    }
}

/*
 * Checks if the the player can move numMoves spaces forward.
 * Returns true if movement is valid, false if not.
 * Will return false if line of movement is not open (block 1 space away when trying to move 2).
 */
bool World::validateAction(Player *player, int numMoves)
{
    sf::Vector2i tilePos;
    /*
     * check every tile up to numMoves
     */
    for(int j = 1; j <= numMoves; j++)
    {
        tilePos = player->getTilePos(j);
        if(tilePos.y < 0 || tilePos.y > mapTileHeight - 1)
            return false;
        if(tilePos.x < 0 || tilePos.x > mapTileWidth - 1)
            return false;
        if(map->getChildNode(MapCreator::get1d(tilePos.x, tilePos.y, mapTileWidth),
                Category::Type::MOVEMENT_SPACE) == NULL)
            return false;
    }

    return true;
}

//CommandQueue* World::getCommandQueue()
//{
//	return mCommandQueue;
//}

void World::loadTextures()
{
    textures.load(Textures::BUTTONS, "Media/Textures/Buttons.png");
    textures.load(Textures::BOARD, "Media/Textures/Board.png");
    textures.load(Textures::TRANKS, "Media/Textures/Tranks.png");
    textures.load(Textures::BULLETS, "Media/Textures/Bullets.png");
}

void World::adaptPlayerPosition()
{
    // Keep player's position inside the screen bounds, at least borderDistance units from the border
    sf::FloatRect viewBounds = getViewBounds();
    const float borderDistance = 40.f;

//	sf::Vector2f position = mPlayerAircraft->getPosition();
//	position.x = std::max(position.x, viewBounds.left + borderDistance);
//	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
//	position.y = std::max(position.y, viewBounds.top + borderDistance);
//	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
//	mPlayerAircraft->setPosition(position);
}

void World::adaptPlayerVelocity()
{
//	sf::Vector2f velocity = mPlayerAircraft->getVelocity();

// If moving diagonally, reduce velocity (to have always same velocity)
//	if (velocity.x != 0.f && velocity.y != 0.f)
//		mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));

// Add scrolling velocity
//	mPlayerAircraft->accelerate(0.f, mScrollSpeed);
}

void World::handleEvent(const sf::Event* event)
{
    GUI::Component* selected;
    if(event->type == sf::Event::MouseButtonPressed || event->type == sf::Event::MouseButtonReleased)
    {
        if(!trankControls.handleEvent(event))
        {
            if(sendCommandBox.handleEvent(event))
            {
                if((selected = trankControls.getSelectedComponent()) != NULL)
                    ((GUI::Button*) selected)->trigger();
                else
                    std::cout << "Nothing selected\n";
            }
        }
    }
}

void World::buildScene()
{
    MapCreator mc(&textures);

    State::Context tmpContext(window, &textures, fonts);

    //TODO Make this an ID like textures and fonts
    mc.generate("Media/Maps/test.xml");
    map = mc.getMap();
    sceneGraph.attachChild(map);
    mapTileWidth = mc.getMapWidth();
    mapTileHeight = mc.getMapHeight();
    int buttonX = 1300;
    int buttonY = 200;

    tileSize = mc.getTileWidth();

    for(int j = 0; j < numPlayers; j++)
    {
        //Set map boarders
        players[j]->setMapWidth(mapTileWidth);
        players[j]->setMapHeight(mapTileHeight);
        //Set player spawn position to the vector location
        players[j]->setTileSize(tileSize);
        players[j]->setSpawnPos(mc.getPlayerSpawnPos(j), mc.getPlayerSpawnFacing(j));
        players[j]->setMap(map);

        //The player's spawn is in screen coordinates but in order to place it in the screen graph, it needs
        //the vector position, so get spawn position from mapCreator
        if(!map->layerChildNode(players[j],
                MapCreator::get1d(mc.getPlayerSpawnPos(j).x, mc.getPlayerSpawnPos(j).y, mapTileWidth)))
            std::cout << "you dun fuck up\n";
    }

    //Button initialization
    //TODO free the buttons in the button container
    GUI::Button *tmpButton = new GUI::Button(tmpContext, GUI::ButtonTypes::MOVE_DOUBLE, buttonX, buttonY, 100, 100);
    tmpButton->setCallback([&] ()
    {
        sendMoveToServer(GUI::ButtonTypes::MOVE_DOUBLE);
        //Commented out single player setup
//        pendingPlayerCommands[0] = GUI::ButtonTypes::MOVE_DOUBLE;
//        playersReady++;
//        generatePlayerMoves();
        });
    trankControls.pack(tmpButton);
    buttonY += 100;

    tmpButton = new GUI::Button(tmpContext, GUI::ButtonTypes::MOVE_SINGLE, buttonX, buttonY, 100, 100);
    tmpButton->setCallback([&] ()
    {
        sendMoveToServer(GUI::ButtonTypes::MOVE_SINGLE);
//        pendingPlayerCommands[0] = GUI::ButtonTypes::MOVE_SINGLE;
//        playersReady++;
//        generatePlayerMoves();
        });
    trankControls.pack(tmpButton);
    buttonX += 100;

    tmpButton = new GUI::Button(tmpContext, GUI::ButtonTypes::CHECK_BOX, buttonX, buttonY, 100, 100);
    tmpButton->setCallback([&] ()
    {
        sendMoveToServer(GUI::ButtonTypes::CHECK_BOX);
//        pendingPlayerCommands[0] = GUI::ButtonTypes::CHECK_BOX;
//        playersReady++;
//        generatePlayerMoves();
        });
    sendCommandBox.pack(tmpButton);
    buttonX -= 100;
    buttonY += 100;

    tmpButton = new GUI::Button(tmpContext, GUI::ButtonTypes::ROTATE_FULL, buttonX, buttonY, 100, 100);
    tmpButton->setCallback([&] ()
    {
        sendMoveToServer(GUI::ButtonTypes::ROTATE_FULL);
//        pendingPlayerCommands[0] = GUI::ButtonTypes::ROTATE_FULL;
//        playersReady++;
//        generatePlayerMoves();
        });
    trankControls.pack(tmpButton);
    buttonX -= 100;

    tmpButton = new GUI::Button(tmpContext, GUI::ButtonTypes::ROTATE_HALF_COUNTER, buttonX, buttonY, 100, 100);
    tmpButton->setCallback([&] ()
    {
        sendMoveToServer(GUI::ButtonTypes::ROTATE_HALF_COUNTER);
//        pendingPlayerCommands[0] = GUI::ButtonTypes::ROTATE_HALF_COUNTER;
//        playersReady++;
//        generatePlayerMoves();
        });
    trankControls.pack(tmpButton);
    buttonX += 200;

    tmpButton = new GUI::Button(tmpContext, GUI::ButtonTypes::ROTATE_HALF_CLOCKWISE, buttonX, buttonY, 100, 100);
    tmpButton->setCallback([&] ()
    {
        sendMoveToServer(GUI::ButtonTypes::ROTATE_HALF_CLOCKWISE);
//        pendingPlayerCommands[0] = GUI::ButtonTypes::ROTATE_HALF_CLOCKWISE;
//        playersReady++;
//        generatePlayerMoves();
        });
    trankControls.pack(tmpButton);
    buttonX -= 100;
    buttonY += 100;

    tmpButton = new GUI::Button(tmpContext, GUI::ButtonTypes::FIRE, buttonX, buttonY, 100, 100);
    tmpButton->setCallback([&] ()
    {
        sendMoveToServer(GUI::ButtonTypes::FIRE);
//        pendingPlayerCommands[0] = GUI::ButtonTypes::FIRE;
//        playersReady++;
//        generatePlayerMoves();
        });
    trankControls.pack(tmpButton);

//    trankControls.pack()

//    sf::RenderTexture bgTexture;
//    bgTexture.create(mc.getWidth() * 75, mc.getHeight() * 75);

//    bgTexture.draw(*board);
//    bgTexture.display();

    // Initialize the different layers
//	for (std::size_t i = 0; i < LAYERCOUNT; ++i)
//	{
//		Category::Type category = (i == LowerAir) ? Category::SceneAirLayer : Category::NONE;
//
//		SceneNode::Ptr layer(new SceneNode(category));
//		sceneLayers[i] = layer.get();
//
//		sceneGraph.attachChild(std::move(layer));
//	}
//
//	// Prepare the tiled background
//	sf::Texture& jungleTexture = textures.get(Textures::Jungle);
//	jungleTexture.setRepeated(true);
//
//	float viewHeight = worldView.getSize().y;
//	sf::IntRect textureRect(worldBounds);
//	textureRect.height += static_cast<int>(viewHeight);
//
//	// Add the background sprite to the scene
//	std::unique_ptr<SpriteNode> jungleSprite(new SpriteNode(jungleTexture, textureRect));
//	jungleSprite->setPosition(worldBounds.left, worldBounds.top - viewHeight);
//	sceneLayers[Background]->attachChild(std::move(jungleSprite));
//
//	// Add the finish line to the scene
//	sf::Texture& finishTexture = textures.get(Textures::FinishLine);
//	std::unique_ptr<SpriteNode> finishSprite(new SpriteNode(finishTexture));
//	finishSprite->setPosition(0.f, -76.f);
//	sceneLayers[Background]->attachChild(std::move(finishSprite));
//
//	// Add particle node to the scene
//	std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(Particle::Smoke, textures));
//	sceneLayers[LowerAir]->attachChild(std::move(smokeNode));
//
//	// Add propellant particle node to the scene
//	std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(Particle::Propellant, textures));
//	sceneLayers[LowerAir]->attachChild(std::move(propellantNode));
//
//	// Add sound effect node
//	std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
//	sceneGraph.attachChild(std::move(soundNode));
//
//	// Add player's aircraft
//	std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, textures, fonts));
//	mPlayerAircraft = player.get();
//	mPlayerAircraft->setPosition(mSpawnPosition);
//	sceneLayers[UpperAir]->attachChild(std::move(player));
//
//	// Add enemy aircraft
//	addEnemies();
}

sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect(worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
    // Return view bounds + some area at top, where enemies spawn
    sf::FloatRect bounds = getViewBounds();
    bounds.top -= 100.f;
    bounds.height += 100.f;

    return bounds;
}

/*
 * Used to fill player 1-3 commands with a random direction movement
 */
void World::generatePlayerMoves()
{
    for(int j = 1; j < numPlayers; j++)
    {
        int randNum = rand() % 6;

        //If 5 is selected, the command is shoot, don't let computers shoot
        if(randNum == 5)
            randNum = 6;

        GUI::ButtonTypes command = static_cast<GUI::ButtonTypes>(5);
        pendingPlayerCommands[j] = command;
        playersReady++;
    }
}

void World::sendMoveToServer(GUI::ButtonTypes action)
{
    sf::Packet packet;

    //doing nothing does not send a command to do nothing
    packet << INT32(Client::ACTION) << INT32(playerId) << INT16(action);
    socket->send(packet);
}
