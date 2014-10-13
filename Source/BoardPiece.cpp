#include "BoardPiece.hpp"
#include "SceneNode.hpp"
#include "DataTables.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <stdlib.h>
#include <iostream>

BoardPiece::BoardPiece(BoardPiece::Type pieceType, TextureHolder* textures, float x, float y) :
        BoardPiece(pieceType, textures, x, y, 0)
{
}

BoardPiece::BoardPiece(BoardPiece::Type pieceType, TextureHolder* textures, float x, float y, float angle) :
        SceneNode(), type(pieceType)
{
    TextureData* table = initializeBoardPieceData();
    sprite = MySprite(textures->get(table[pieceType].textureId), table[pieceType].textureRect);
    sprite.centerOrigin();
    sprite.rotate(angle);
    sprite.setPosition(x, y);
    free(table);
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
