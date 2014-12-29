#include "Player.hpp"
#include "DataTables.hpp"
#include "Utils.hpp"
#include <iostream>

Player::Player(int playerNum, TextureHolder* textures) :
                moveLimit(0),
                forward(0),
                spawnPos(),
                SceneNode(Category::PLAYER),
                spawnFacing(NORTH),
                rotating(false),
                moving(false),
                rotationDir(CLOCKWISE),
                rotationLimit(0),
                tileWidth(-1),
                mapWidth(0),
                mapHeight(0)
{
    TextureData* table = initializePlayerData();
    sprite = MySprite(textures->get(table[playerNum].textureId), table[playerNum].textureRect);
    sprite.centerOrigin();
    sprite.setPosition(0, 0);
    //TODO change to vector. yo
    free(table);
}

Player::~Player()
{
}

/*
 * If the Trank is moving or rotating, update that information here
 */
void Player::updateCurrent(sf::Time dt)
{
    if(rotating)
    {
        forward += dt.asMilliseconds() * ROTATION_SPEED * rotationDir;
        if(rotationDir > 0 && forward > rotationLimit)
        {
            rotating = false;
            forward = rotationLimit;
            //set forward to a cardinal direction to prevent rounding errors later
            forward = getForwardDirection();
        }
        else if(rotationDir < 0 && forward < rotationLimit)
        {
            rotating = false;
            forward = rotationLimit;
            //set forward to a cardinal direction to prevent rounding errors later
            forward = getForwardDirection();
        }

        sprite.setRotation(forward);
    }
    else if(moving)
    {
        //TODO find better way to do this?
        switch(getForwardDirection())
        {
        case Direction::NORTH:
            sprite.move(0, -1 * dt.asMilliseconds() * MOVEMENT_SPEED);
            if(sprite.getPosition().y < moveLimit)
            {
                sprite.setPosition(sprite.getPosition().x, moveLimit);
                moving = false;
            }
            break;
        case Direction::SOUTH:
            sprite.move(0, dt.asMilliseconds() * MOVEMENT_SPEED);
            if(sprite.getPosition().y > moveLimit)
            {
                sprite.setPosition(sprite.getPosition().x, moveLimit);
                moving = false;
            }
            break;
        case Direction::EAST:
            sprite.move(dt.asMilliseconds() * MOVEMENT_SPEED, 0);
            if(sprite.getPosition().x > moveLimit)
            {
                sprite.setPosition(moveLimit, sprite.getPosition().y);
                moving = false;
            }
            break;
        case Direction::WEST:
            sprite.move(-1 * dt.asMilliseconds() * MOVEMENT_SPEED, 0);
            if(sprite.getPosition().x < moveLimit)
            {
                sprite.setPosition(moveLimit, sprite.getPosition().y);
                moving = false;
            }
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
//    tilePos = spawnPos;
    forward = spawnFacing;
    sprite.setPosition(sf::Vector2f(spawnPos.x * tileWidth, spawnPos.y * tileWidth));
}

/*
 * Used to set the spawn position of the player
 */
void Player::setSpawnPos(sf::Vector2i pos, Direction facing)
{
//TODO figure out if copy constructors work this way
    spawnPos = pos;
    tilePos = pos;
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

    switch(getForwardDirection())
    {
    case Direction::NORTH:
        moveLimit = sprite.getPosition().y - limit;
        tilePos.y -= limit / Player::SINGLE_MOVE;
        break;
    case Direction::SOUTH:
        moveLimit = sprite.getPosition().y + limit;
        tilePos.y += limit / Player::SINGLE_MOVE;
        break;
    case Direction::EAST:
        moveLimit = sprite.getPosition().x + limit;
        tilePos.x += limit / Player::SINGLE_MOVE;
        break;
    case Direction::WEST:
        moveLimit = sprite.getPosition().x - limit;
        tilePos.x -= limit / Player::SINGLE_MOVE;
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
    switch(getForwardDirection())
    {
    case Direction::NORTH:
        if(tilePos.y - spacesMoved < 0)
            return false;
        break;
    case Direction::SOUTH:
        if(tilePos.y + spacesMoved >= mapHeight)
            return false;
        break;
    case Direction::EAST:
        if(tilePos.x + spacesMoved >= mapWidth)
            return false;
        break;
    case Direction::WEST:
        if(tilePos.x - spacesMoved < 0)
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

Player::Direction Player::getForwardDirection()
{
    if(mod(forward, Direction::NORTH) == 0)
        return Direction::NORTH;
    else if(mod(forward, Direction::SOUTH) == 0)
        return Direction::SOUTH;
    else if(mod(forward, Direction::WEST) == 0)
        return Direction::WEST;
    return Direction::EAST;
}

sf::Vector2i Player::getTilePos()
{
    return tilePos;
}

/*
 * Returns space player will be if moved numSpaces forward
 */
sf::Vector2i Player::getTilePos(int numSpaces)
{
    switch(getForwardDirection())
    {
    case Player::Direction::NORTH:
        return sf::Vector2i(tilePos.x, tilePos.y - numSpaces);
    case Player::Direction::SOUTH:
        return sf::Vector2i(tilePos.x, tilePos.y + numSpaces);
    case Player::Direction::EAST:
        return sf::Vector2i(tilePos.x + numSpaces, tilePos.y);
    case Player::Direction::WEST:
        return sf::Vector2i(tilePos.x - numSpaces, tilePos.y);
    }
    return tilePos;
}

/*
 * Returns true if player is moving, rotating, or shooting. Otherwise false
 */
bool Player::isActionExecuting()
{
    return rotating || moving;
}
