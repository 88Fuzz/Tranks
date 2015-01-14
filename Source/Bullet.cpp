#include "Bullet.hpp"
#include "DataTables.hpp"
#include "Utils.hpp"
#include "MapCreator.hpp"
#include <iostream>

Bullet::Bullet(int playerNum, TextureHolder *textures) :
        bounceTotal(BOUNCE_LIMIT + 1),
        Moveable(Category::PLAYER, 0, 0, -1, Direction::NORTH)
{
    TextureData* table = initializeBulletData();
    sprite = MySprite(textures->get(table[playerNum].textureId), table[playerNum].textureRect);
    sprite.centerOrigin();
    sprite.setPosition(0, 0);
    yFactor = 0;
    xFactor = 0;

    //TODO change to vector. yo
    free(table);
}

Bullet::~Bullet()
{

}

void Bullet::resetBounceTotal()
{
    bounceTotal = 0;
}

/*
 * Used to set the spawn position of the player
 */
void Bullet::setSpawnPos(sf::Vector2i pos, Direction facing)
{
//TODO figure out if copy constructors work this way
    tilePos = pos;

    sf::Vector2f spritePos;
    float xPos = pos.x * tileSize;
    float yPos = pos.y * tileSize;
    float offset = tileSize / 2 - sprite.getWidth() / 2;

    xFactor = 0;
    yFactor = 0;
    switch(getForwardDirection())
    {
    case NORTH:
        yFactor = -1;
        spritePos = sf::Vector2f(xPos + offset, yPos + tileSize);
        break;
    case SOUTH:
        yFactor = 1;
        spritePos = sf::Vector2f(xPos + offset, yPos);
        break;
    case EAST:
        xFactor = 1;
        spritePos = sf::Vector2f(xPos, yPos + offset);
        break;
    case WEST:
        xFactor = -1;
        spritePos = sf::Vector2f(xPos + tileSize, yPos + offset);
        break;
    }

    sprite.setPosition(spritePos);
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
    float movement = dt.asMilliseconds() * MOVEMENT_SPEED;
    sprite.move(xFactor * movement, yFactor * movement);

    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2i posI = sf::Vector2i(pos.x, pos.y);
    sf::Vector2i posWidthI = sf::Vector2i(pos.x + sprite.getWidth(), pos.y + sprite.getHeight());

    posI /= tileSize;
    posWidthI /= tileSize;

    //Override if different, the two should never both be different than the current tilePos
    if(posWidthI != posI)
        posI = posWidthI;

    //Bullet has moved out of current tile
    if(tilePos != posI)
    {
        if(posI.x < 0)
        {
            posI.x = 0;
            rotate(180);
        }
        else if(posI.x >= getMapWidth())
        {
            posI.x = getMapWidth() - 1;
            rotate(180);
        }
        else if(posI.y < 0)
        {
            posI.y = 0;
            rotate(180);
        }
        else if(posI.y >= getMapHeight())
        {
            posI.y = getMapHeight() - 1;
            rotate(180);
        }

        tilePos = posI;
    }
//    std::cout << "Current tilePos x " << tilePos.x << " " << tilePos.y << "\n";
//    std::cout << "\t1d: " << MapCreator::get1d(tilePos.x, tilePos.y, tileSize) << "\n";
//    std::cout << "new tilePos x " << posI.x << " " << posI.y << "\n";
//    std::cout << "\t1d: " << MapCreator::get1d(posI.x, posI.y, tileSize) << "\n";
}

/*
 * Rotates the sprite by rotation degrees. (-90, 90, 180).
 *
 * Returns true if limit of number of rotations is reached
 */
void Bullet::rotate(int rotation)
{
    sprite.rotate(rotation);

    bounceTotal += rotation;

    switch(getForwardDirection())
    {
    //TODO there's probably a better way to do this
    case Direction::NORTH:
        if(rotation == 90)
        {
            forward = Direction::EAST;
            yFactor = 0;
            xFactor = 1;
        }
        else if(rotation == 180)
        {
            forward = Direction::SOUTH;
            yFactor *= -1;
            xFactor *= -1;
        }
        else
        {
            forward = Direction::EAST;
            yFactor = 0;
            xFactor = -1;
        }
        break;
    case Direction::SOUTH:
        if(rotation == 90)
        {
            forward = Direction::WEST;
            yFactor = 0;
            xFactor = -1;
        }
        else if(rotation == 180)
        {
            forward = Direction::NORTH;
            yFactor *= -1;
            xFactor *= -1;
        }
        else
        {
            forward = Direction::EAST;
            yFactor = 0;
            xFactor = 1;
        }
        break;
    case Direction::EAST:
        if(rotation == 90)
        {
            forward = Direction::SOUTH;
            yFactor = 1;
            xFactor = 0;
        }
        else if(rotation == 180)
        {
            forward = Direction::WEST;
            yFactor *= -1;
            xFactor *= -1;
        }
        else
        {
            forward = Direction::NORTH;
            yFactor = -1;
            xFactor = 0;
        }
        break;
    case Direction::WEST:
        if(rotation == 90)
        {
            forward = Direction::NORTH;
            yFactor = -1;
            xFactor = 0;
        }
        else if(rotation == 180)
        {
            forward = Direction::EAST;
            yFactor *= -1;
            xFactor *= -1;
        }
        else
        {
            forward = Direction::SOUTH;
            yFactor = 1;
            xFactor = 0;
        }
        break;
    }
}

bool Bullet::isAlive()
{
    return bounceTotal <= BOUNCE_LIMIT;
}
