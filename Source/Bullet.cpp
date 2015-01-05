#include "Bullet.hpp"
#include "DataTables.hpp"
#include "Utils.hpp"
#include <iostream>

Bullet::Bullet(sf::Vector2i pos, int tileSize, Player::Direction direction, int playerNum, TextureHolder *textures) :
        tileWidth(tileSize), forward(direction)
{
    TextureData* table = initializeBulletData();
    sprite = MySprite(textures->get(table[playerNum].textureId), table[playerNum].textureRect);
    sprite.centerOrigin();
    sprite.setPosition(0, 0);

    //TODO change to vector. yo
    free(table);

    setSpawnPos(pos, direction);
}

Bullet::~Bullet()
{

}

/*
 * Used to set the spawn position of the player
 */
void Bullet::setSpawnPos(sf::Vector2i pos, Player::Direction facing)
{
//TODO figure out if copy constructors work this way
    tilePos = pos;
    sprite.setPosition(sf::Vector2f(pos.x * tileWidth, pos.y * tileWidth));
    forward = facing;
    sprite.setRotation(forward);
}

void Bullet::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite);
}

void Bullet::updateCurrent(sf::Time dt)
{
    //TODO this will break when rotation and zooming
    //TODO find better way to do this?
    //TODO have x and y as variables -1, 0, or 1 that changes when direction changes
    switch(getForwardDirection())
    {
    case Player::Direction::NORTH:
        sprite.move(0, -1 * dt.asMilliseconds() * MOVEMENT_SPEED);
        break;
    case Player::Direction::SOUTH:
        sprite.move(0, dt.asMilliseconds() * MOVEMENT_SPEED);
        break;
    case Player::Direction::EAST:
        sprite.move(dt.asMilliseconds() * MOVEMENT_SPEED, 0);
        break;
    case Player::Direction::WEST:
        sprite.move(-1 * dt.asMilliseconds() * MOVEMENT_SPEED, 0);
        break;
    default:
        //TODO LOGGING'
        std::cerr << "forward angle of " << forward << " not found\n";
    }
}

Player::Direction Bullet::getForwardDirection()
{
    if(mod(forward, Player::Direction::NORTH) == 0)
        return Player::Direction::NORTH;
    else if(mod(forward, Player::Direction::SOUTH) == 0)
        return Player::Direction::SOUTH;
    else if(mod(forward, Player::Direction::WEST) == 0)
        return Player::Direction::WEST;
    return Player::Direction::EAST;
}
