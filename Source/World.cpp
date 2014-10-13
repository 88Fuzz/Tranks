#include "World.hpp"
#include "MapCreator.hpp"
#include "Button.hpp"
#include "ButtonTypes.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>

//TODO clean up this code

World::World(sf::RenderWindow* outputTarget, FontHolder* fonts) :
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
                sendCommandBox()
{
    sceneTexture.create(window->getSize().x, window->getSize().y);

    loadTextures();

    for(int j = 0; j < MapCreator::PLAYER_COUNT; j++)
        players.push_back(new Player(j, &textures));

    buildScene();
    //TODO figure out if this is needed
    // Prepare the view
//	worldView.setCenter(mSpawnPosition);
}

World::~World()
{
    if(map != NULL)
        delete map;

    for(int j = 0; j < players.size(); j++)
        delete players[j];
}

void World::update(sf::Time dt)
{
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

//CommandQueue* World::getCommandQueue()
//{
//	return mCommandQueue;
//}

void World::loadTextures()
{
    textures.load(Textures::BUTTONS, "Media/Textures/Buttons.png");
    textures.load(Textures::BOARD, "Media/Textures/Board.png");
    textures.load(Textures::TRANKS, "Media/Textures/Tranks.png");
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
    if(event->type == sf::Event::MouseButtonPressed || event->type == sf::Event::MouseButtonReleased)
    {
        if(!trankControls.handleEvent(event))
            sendCommandBox.handleEvent(event);
    }
//        while(window.pollEvent(event))
//                {
//                    if(event.type == sf::Event::MouseWheelMoved)
//                    {
//                        std::cout << "movement " << event.mouseWheel.delta << "\n";
//                        std::cout << "mouse x: " << sf::Mouse::getPosition(window).x << " y: "
//                                << sf::Mouse::getPosition(window).y << "\n";
//                    }
//                    if(event.type == sf::Event::Closed)
//                        window.close();
//
//                    if(event.type == sf::Event::Closed
//                            || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
//                        window.close();
//                }
}

void World::buildScene()
{
    MapCreator mc(&textures);

    State::Context tmpContext(window, &textures, fonts);

    //TODO Make this an ID like textures and fonts
    mc.generate("Media/Maps/test.xml");
//    BoardPiece *board = mc.getMap();
    map = mc.getMap();
    sceneGraph.attachChild(map);
    int buttonX = 1600;
    int buttonY = 200;

    for(int j = 0; j < players.size(); j++)
    {
        //Set map boarders
        players[j]->setMapWidth(mc.getMapWidth());
        players[j]->setMapHeight(mc.getMapHeight());
        //Set player spawn position to the vector location
        players[j]->setTileWidth(mc.getTileWidth());
        players[j]->setSpawnPos(mc.getPlayerSpawnPos(j), mc.getPlayerSpawnFacing(j));

        //The player's spawn is in screen coordinates but in order to place it in the screen graph, it needs
        //the vector position, so get spawn position from mapCreator
        if(!map->layerChildNode(players[j], mc.get1d(mc.getPlayerSpawnPos(j).x, mc.getPlayerSpawnPos(j).y)))
            std::cout << "you dun fuck up\n";
//        map.layerChildNode(&players[j], mc.get1d(players[j].getSpawnPositionX(), players[j].getSpawnPositionY()));
    }
//    players[0]->startRotation(Player::Direction::CLOCKWISE, Player::SINGLE_ROTATION);
    players[0]->startMovement(Player::SINGLE_MOVE);
//    players[1]->startRotation(Player::Rotation::CLOCKWISE, Player::SINGLE_ROTATION);
    players[1]->startMovement(Player::SINGLE_MOVE);
//    players[2]->startRotation(Player::Direction::COUNTER_CLOCKWISE, Player::SINGLE_ROTATION);
    players[2]->startMovement(Player::SINGLE_MOVE);
//    players[3]->startRotation(Player::Rotation::COUNTER_CLOCKWISE, Player::DOUBLE_ROTATION);
    players[3]->startMovement(Player::SINGLE_MOVE);

    //Button initialization
    GUI::Button *tmpButton = new GUI::Button(tmpContext, GUI::TRANK_CONTROLS::MOVE_DOUBLE, buttonX, buttonY, 100, 100);
    trankControls.pack(tmpButton);
    buttonY += 100;
    tmpButton = new GUI::Button(tmpContext, GUI::TRANK_CONTROLS::MOVE_SINGLE, buttonX, buttonY, 100, 100);
    trankControls.pack(tmpButton);
    buttonX += 100;

    tmpButton = new GUI::Button(tmpContext, GUI::TRANK_CONTROLS::CHECK_BOX, buttonX, buttonY, 100, 100);
    sendCommandBox.pack(tmpButton);
    buttonX -= 100;
    buttonY += 100;

    tmpButton = new GUI::Button(tmpContext, GUI::TRANK_CONTROLS::ROTATE_FULL, buttonX, buttonY, 100, 100);
    trankControls.pack(tmpButton);
    buttonX -= 100;
    tmpButton = new GUI::Button(tmpContext, GUI::TRANK_CONTROLS::ROTATE_HALF_COUNTER, buttonX, buttonY, 100, 100);
    trankControls.pack(tmpButton);
    buttonX += 200;
    tmpButton = new GUI::Button(tmpContext, GUI::TRANK_CONTROLS::ROTATE_HALF_CLOCKWISE, buttonX, buttonY, 100, 100);
    trankControls.pack(tmpButton);
    buttonX -= 100;
    buttonY += 100;
    tmpButton = new GUI::Button(tmpContext, GUI::TRANK_CONTROLS::FIRE, buttonX, buttonY, 100, 100);
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
