#include "Player.hpp"
#include "DataTables.hpp"
#include "Utils.hpp"
#include <iostream>

/*
 * Used to construct a player with no useful information.
 */
Player::Player() :
        SceneNode()
{

}

Player::~Player()
{
}

Player::Player(int playerNum, TextureHolder* textures) :
                moveLimit(0),
                forward(0),
                spawnPos(),
                SceneNode(),
                spawnFacing(NORTH),
                rotating(false),
                moving(false),
                rotationDir(CLOCKWISE),
                rotationLimit(0),
                tileWidth(-1)
{
    TextureData* table = initializePlayerData();
    sprite = MySprite(textures->get(table[playerNum].textureId), table[playerNum].textureRect);
    sprite.centerOrigin();
    sprite.setPosition(0, 0);
    //TODO change to vector. yo
    free(table);
}

/*
 * If the Trank is moving or rotating, update that information here
 */
void Player::updateCurrent(sf::Time dt)
{
    if(rotating)
    {
        forward += dt.asMilliseconds() * ROTATION_SPEED * rotationDir;
        sprite.setRotation(forward);

        if(rotationDir > 0 && forward > rotationLimit)
        {
            rotating = false;
            forward = rotationLimit;
        }
        else if(rotationDir < 0 && forward < rotationLimit)
        {
            rotating = false;
            forward = rotationLimit;
        }
    }
    else if(moving)
    {
        //TODO find better way to do this?
        switch((int) forward)
        {
        case Direction::NORTH:
            sprite.move(0, -1 * dt.asMilliseconds() * MOVEMENT_SPEED);
            if(sprite.getPosition().y < moveLimit)
                moving = false;
            break;
        case Direction::SOUTH:
            sprite.move(0, dt.asMilliseconds() * MOVEMENT_SPEED);
            if(sprite.getPosition().y > moveLimit)
                moving = false;
            break;
        case Direction::EAST:
            sprite.move(dt.asMilliseconds() * MOVEMENT_SPEED, 0);
            if(sprite.getPosition().x > moveLimit)
                moving = false;
            break;
        case Direction::WEST:
            sprite.move(-1 * dt.asMilliseconds() * MOVEMENT_SPEED, 0);
            if(sprite.getPosition().x < moveLimit)
                moving = false;
            break;
        default:
            //TODO LOGGING'
            std::cerr << "forward angle of " << forward << " not found\n";
        }
    }
}

void Player::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite);
}

/*
 * Tiles should be square so only width is needed
 */
void Player::setTileWidth(int width)
{
    tileWidth = width;
}

void Player::setMapHeight(int height)
{
    mapHeight = height;
}
void Player::setMapWidth(int width)
{
    mapWidth = width;
}

/*
 * resets the players position and direction facing to the spawn point
 */
void Player::respawn()
{
    currentPos = spawnPos;
    forward = spawnFacing;
    sprite.setPosition(sf::Vector2f(currentPos.x * tileWidth, currentPos.y * tileWidth));
}

/*
 * Used to set the spawn position of the player
 */
void Player::setSpawnPos(sf::Vector2i pos, Direction facing)
{
//TODO figure out if copy constructors work this way
    spawnPos = pos;
    currentPos = pos;
    sprite.setPosition(sf::Vector2f(pos.x * tileWidth, pos.y * tileWidth));
    forward = facing;
    spawnFacing = facing;
    sprite.setRotation(forward);
}

/*
 * Used to initialize the Tranks rotation, limit is how much the Trank should rotate
 */
void Player::startRotation(Rotation rotateDir, int limit)
{
    if(rotating)
        //TODO logging
        std::cerr << "Already rotating, game in bad state\n";

    rotationDir = rotateDir;
    rotationLimit = forward + rotateDir * limit;
    rotating = true;
}

/*
 * Used to initialize the Tranks movement, limit is how far the Trank should move
 */
void Player::startMovement(int limit)
{
    int tileLimit;
    if(moving)
        //TODO logging
        std::cerr << "Already moving, game in bad state\n";

//TODO THIS WILL BREAK HORRIBLY ONCE YOU GET MAP MOVEMENTS!
//TODO THIS WILL BREAK HORRIBLY ONCE YOU GET MAP MOVEMENTS!
//TODO THIS WILL BREAK HORRIBLY ONCE YOU GET MAP MOVEMENTS!
//TODO THIS WILL BREAK HORRIBLY ONCE YOU GET MAP MOVEMENTS!
//TODO THIS WILL BREAK HORRIBLY ONCE YOU GET MAP MOVEMENTS!
//TODO THIS WILL BREAK HORRIBLY ONCE YOU GET MAP MOVEMENTS!
//TODO THIS WILL BREAK HORRIBLY ONCE YOU GET MAP MOVEMENTS!
//TODO THIS WILL BREAK HORRIBLY ONCE YOU GET MAP MOVEMENTS!
//TODO THIS WILL BREAK HORRIBLY ONCE YOU GET MAP MOVEMENTS!
//TODO THIS WILL BREAK HORRIBLY ONCE YOU GET MAP MOVEMENTS!

    if(limit == DOUBLE_MOVE)
        tileLimit = 2;
    else
        tileLimit = 1;

    if(!checkValidMove(tileLimit))
    {
        std::cout << " DON'T MOVE!\n";
        return;
    }

    switch((int) forward)
    {
    case Direction::NORTH:
        moveLimit = sprite.getPosition().y - limit;
        break;
    case Direction::SOUTH:
        moveLimit = sprite.getPosition().y + limit;
        break;
    case Direction::EAST:
        moveLimit = sprite.getPosition().x + limit;
        break;
    case Direction::WEST:
        moveLimit = sprite.getPosition().x - limit;
        break;
    default:
        std::cerr << "Error with the facing direction\n";
        break;
    }

    moving = true;
}

int Player::getSpawnPosX()
{
    return spawnPos.x;
}

int Player::getSpawnPosY()
{
    return spawnPos.y;
}

//TODO remove
//Movement validation will be done at the point when all players have submitted the commands for the next turn
bool Player::checkValidMove(int spacesMoved)
{
    //Check if position is in the bounds of the map
    switch((int) forward)
    {
    case Direction::NORTH:
        if(currentPos.y - spacesMoved < 0)
            return false;
        break;
    case Direction::SOUTH:
        if(currentPos.y + spacesMoved >= mapHeight)
            return false;
        break;
    case Direction::EAST:
        if(currentPos.x + spacesMoved >= mapWidth)
            return false;
        break;
    case Direction::WEST:
        if(currentPos.x - spacesMoved < 0)
            return false;
        break;
    default:
        //TODO logging
        std::cerr << "invalid direction facing\n";
        break;
    }

    //TODO check if space is empty tile
    return true;
}
