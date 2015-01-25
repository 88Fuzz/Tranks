#include "Bullet.hpp"
#include "DataTables.hpp"
#include "Utils.hpp"
#include "MapCreator.hpp"
#include <stdlib.h>
#include <iostream>

#define PEEE(x) if(playerNum==0) \
    x

Bullet::Bullet(int playerNum, TextureHolder *textures) :
                bounceTotal(BOUNCE_LIMIT + 1),
                Moveable(Category::PLAYER, 0, 0, -1, Direction::NORTH),
                map(NULL),
                tileDeflect(0),
                tileDeflectFlag(false),
                deflection(0),
                playerNum(playerNum)
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

    //This is stupid
    if(tileDeflectFlag)
    {
        tileDeflect += abs(xFactor * movement + yFactor * movement);

        if(tileDeflect > tileSize / 2)
        {
            tileDeflectFlag = false;
            tileDeflect = 0;
            rotate(deflection);
        }
    }

    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2i posI = sf::Vector2i(pos.x, pos.y);
    sf::Vector2i tmpPos;
    sf::Vector2i posWidthI = sf::Vector2i(pos.x + sprite.getWidth(), pos.y + sprite.getHeight());
    Direction dir = getForwardDirection();
    BoardPiece *piece;

    posI /= tileSize;
    posWidthI /= tileSize;
    tmpPos = posI;

//    PEEE(std::cout << "tilePos " << tilePos.x << " " << tilePos.y << " posI " << posI.x << " " << posI.y << " and posWidthI " << posWidthI.x << " " << posWidthI.y << "\n";)

//Override if different, the two should never both be different than the current tilePos
    if(posWidthI != tilePos && (dir == Direction::SOUTH || dir == Direction::EAST))
        tmpPos = posWidthI;

    //Bullet has moved out of current tile
    if(tilePos != tmpPos)
    {
        if(tmpPos.x < 0)
        {
            tmpPos.x = 0;
            rotate(180);
        }
        else if(tmpPos.x >= getMapWidth())
        {
            tmpPos.x = getMapWidth() - 1;
            rotate(180);
        }
        else if(tmpPos.y < 0)
        {
            tmpPos.y = 0;
            rotate(180);
        }
        else if(tmpPos.y >= getMapHeight())
        {
            tmpPos.y = getMapHeight() - 1;
            rotate(180);
        }
        else if(map->checkTile(MapCreator::get1d(tmpPos.x, tmpPos.y, getMapWidth()), Category::Type::BLOCK) != NULL)
        {
            rotate(180);
        }
        else if((piece = map->checkTile(MapCreator::get1d(tmpPos.x, tmpPos.y, getMapWidth()), Category::Type::DEFLECTOR))
                != NULL && !tileDeflectFlag)
        {
            if((deflection = piece->getDeflection(getForwardDirection())) == 180)
            {
                PEEE(std::cout << "full deflection\n"
                ;
                )
                rotate(deflection); //piece->getDeflection(getForwardDirection()));
            }
            else
            {
                PEEE(std::cout << "half deflection " << tmpPos.x << " " << tmpPos.y << " " << dir << "\n"
                ;
                )
                tileDeflectFlag = true;
            }
        }
        else if((piece = map->checkTile(MapCreator::get1d(tmpPos.x, tmpPos.y, getMapWidth()), Category::Type::PLAYER))
                != NULL)
        {
            this doesn't work'
            std::cout << " PLAYER HIS\n";
            Player * player = (Player *) piece;
            player->setAlive(false);
            //Hit Player!
            if(player->getPlayerNum() != playerNum)
            {
                player->addScore(1);
            }
        }

        tilePos = posI;
    }
}

/*
 * Rotates the sprite by rotation degrees. (-90, 90, 180).
 *
 * Returns true if limit of number of rotations is reached
 */
void Bullet::rotate(int rotation)
{
    sprite.rotate(rotation);

    bounceTotal += abs(rotation);

    PEEE(std::cout << "\trotation " << rotation << " bounceTotal " << bounceTotal << "\n"
    ;
    )

    switch(getForwardDirection())
    {
//TODO there's probably a better way to do this
    case Moveable::Direction::NORTH:
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
            forward = Direction::WEST;
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

void Bullet::setMap(BoardPiece *currMap)
{
    map = currMap;
}
