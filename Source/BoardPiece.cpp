#include "BoardPiece.hpp"
#include "SceneNode.hpp"
#include "DataTables.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <stdlib.h>
#include <iostream>

BoardPiece::BoardPiece(Category::Type pieceType, TextureHolder* textures, float x, float y) :
        BoardPiece(pieceType, textures, x, y, 0)
{
}

BoardPiece::BoardPiece(Category::Type pieceType, TextureHolder* textures, float x, float y, float angle) :
        SceneNode(pieceType)
{
    TextureData* table = initializeBoardPieceData();
    sprite = MySprite(textures->get(table[pieceType].textureId), table[pieceType].textureRect);
    sprite.centerOrigin();
    sprite.rotate(angle);
    sprite.setPosition(x, y);
    //TODO make this vector
    free(table);

    if(pieceType == Category::Type::BLOCK)
    {
        deflections[0] = 180;
        deflections[1] = 180;
        deflections[2] = 180;
        deflections[3] = 180;
    }
    else if(pieceType == Category::Type::DEFLECTOR)
    {
        switch((int) angle)
        {
        case DeflectionDirection::NE:
            debug = "NE";
            deflections[0] = -90;
            deflections[1] = 90;
            deflections[2] = 180;
            deflections[3] = 180;
            break;
        case DeflectionDirection::SE:
            debug = "SE";
            deflections[0] = 180;
            deflections[1] = -90;
            deflections[2] = 90;
            deflections[3] = 180;
            break;
        case DeflectionDirection::SW:
            debug = "SW";
            deflections[0] = 180;
            deflections[1] = 180;
            deflections[2] = -90;
            deflections[3] = 90;
            break;
        case DeflectionDirection::NW:
            debug = "NW";
            deflections[0] = 90;
            deflections[1] = 180;
            deflections[2] = 180;
            deflections[3] = -90;
            break;
        }
    }
}

void BoardPiece::destroy()
{

}

/*
 * Draws boardPiece's sprite on the renderTarget
 */
void BoardPiece::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite);
}

float BoardPiece::getSpriteWidth()
{
    return sprite.getWidth();
}

float BoardPiece::getSpriteHeight()
{
    return sprite.getHeight();
}

/*
 * Used to switch a full tile out.
 * Example: the board is tiled first, then a Bounce block will be inserted
 * over the old tile
 */
int BoardPiece::swapChildNode(SceneNode *node, int pos)
{
    if(children.size() < pos)
        return 0;

    delete children[pos];
    children[pos] = node;
    return 1;
}

/*
 * Used to attach a transparent tile over an existing tile.
 * Example: the spawn pointers are circles that do not take up the full tile.
 * So the spawn point is a child of the base tile point
 */
int BoardPiece::layerChildNode(SceneNode *node, int pos)
{

    if(children.size() < pos)
        return 0;

    children[pos]->attachChild(node);
    return 1;
}

/*
 * Given direction an object is moving (forward), return degrees object has been deflected
 */
int BoardPiece::getDeflection(Moveable::Direction forward)
{
    switch(forward)
    {
    case Moveable::Direction::NORTH:
        std::cout << " north " << deflections[0] << "\n";
        return deflections[0];
    case Moveable::Direction::EAST:
        std::cout << " east " << deflections[1] << "\n";
        return deflections[1];
    case Moveable::Direction::SOUTH:
        std::cout << " south " << deflections[2] << "\n";
        return deflections[2];
    case Moveable::Direction::WEST:
        std::cout << " west " << deflections[3] << "\n";
        return deflections[3];
    }

    return 180;
}
