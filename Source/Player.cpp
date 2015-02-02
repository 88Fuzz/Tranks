#include "Player.hpp"
#include "DataTables.hpp"
#include "Utils.hpp"
#include <iostream>

Player::Player(int playerNum, TextureHolder* textures) :
                moveLimit(0),
                spawnPos(),
                Moveable(Category::PLAYER, 0, 0, -1, Direction::NORTH),
                spawnFacing(Direction::NORTH),
                rotating(false),
                moving(false),
                rotationDir(CLOCKWISE),
                rotationLimit(0),
                playerNumber(playerNum),
                actionExecuting(),
                alive(true),
                playerNum(playerNum),
                score(0),
                deathRespawnCount(0)
{
    TextureData* table = initializePlayerData();
    sprite = MySprite(textures->get(table[playerNum].textureId), table[playerNum].textureRect);
    sprite.centerOrigin();
    sprite.setPosition(0, 0);
    //TODO change to vector. yo
    free(table);
    bullet = new Bullet(playerNum, textures);

    attachChild(bullet);
}

Player::~Player()
{
    delete bullet;
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

    //Check if player's bullet is alive or if it has killed anyone
//    if(!bullet->isAlive())
//    {
//        std::cout << "BULLET IS DEAD\n";
//    }
}

void Player::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite);
    if(bullet->isAlive())
        bullet->drawCurrent(target, states);
}

/*
 * resets the players position and direction facing to the spawn point
 */
void Player::respawn()
{
    deathRespawnCount=0;
    setAlive(true);
    tilePos = spawnPos;
    sprite.setRotation(spawnFacing);
    forward = spawnFacing;
    sprite.setPosition(sf::Vector2f(spawnPos.x * tileSize, spawnPos.y * tileSize));
}

/*
 * Used to set the spawn position of the player
 */
void Player::setSpawnPos(sf::Vector2i pos, Moveable::Direction facing)
{
    spawnPos = pos;
    tilePos = pos;
    sprite.setPosition(sf::Vector2f(pos.x * tileSize, pos.y * tileSize));
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

sf::Vector2i Player::getTilePos()
{
    return tilePos;
}

/*
 * Returns space player will be if moved numSpaces forward. Returned value may not be valid
 */
sf::Vector2i Player::getTilePos(int numSpaces)
{
    switch(getForwardDirection())
    {
    case Direction::NORTH:
        return sf::Vector2i(tilePos.x, tilePos.y - numSpaces);
    case Direction::SOUTH:
        return sf::Vector2i(tilePos.x, tilePos.y + numSpaces);
    case Direction::EAST:
        return sf::Vector2i(tilePos.x + numSpaces, tilePos.y);
    case Direction::WEST:
        return sf::Vector2i(tilePos.x - numSpaces, tilePos.y);
    }
    return tilePos;
}

/*
 * Returns space player will be if moved numSpaces forward. If new tile is out of bounds,
 * the current tilePos will be returned.
 */
sf::Vector2i Player::getTileCorrectedPos(int numSpaces)
{
    int tmpX = tilePos.x;
    int tmpY = tilePos.y;
    switch(getForwardDirection())
    {
    case Direction::NORTH:
        if(tmpY - numSpaces > 0)
            tmpY -= numSpaces;
        break;
    case Direction::SOUTH:
        if(tmpY + numSpaces < mapHeight)
            tmpY += numSpaces;
        break;
    case Direction::EAST:
        if(tmpX + numSpaces < mapWidth)
            tmpX += numSpaces;
        break;
    case Direction::WEST:
        if(tmpX - numSpaces > 0)
            tmpX -= numSpaces;
        break;
    }

    return sf::Vector2i(tmpX, tmpY);
}

/*
 * Returns true if player is moving, rotating, or shooting. Otherwise false
 */
bool Player::isActionExecuting()
{
    return rotating || moving || bullet->isAlive();
}

/*
 * Start what is needed to for the fire action
 */
void Player::startFire()
{
    sf::Vector2i tmpPos = getTileCorrectedPos(1);

    bullet->setMapHeight(mapHeight);
    bullet->setMapWidth(mapWidth);
    bullet->setTileSize(tileSize);

    bullet->resetBounceTotal();
    bullet->setSpawnPos(getTileCorrectedPos(1), getForwardDirection());
}

void Player::setMap(BoardPiece *map)
{
    bullet->setMap(map);
}

bool Player::isAlive()
{
    return alive;
}

void Player::setAlive(bool status)
{
    alive = status;
}

void Player::setScore(int newScore)
{
    score = newScore;
}

void Player::addScore(int num)
{
    score += num;
}

int Player::getScore()
{
    return score;
}

int Player::getPlayerNum()
{
    return playerNum;
}

void Player::incrementDeathClock()
{
    deathRespawnCount++;
    if(deathRespawnCount > RESPAWN_LIMIT)
        respawn();
}
