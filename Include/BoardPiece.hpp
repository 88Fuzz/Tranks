#ifndef __BOARDPIECE_HPP__
#define __BOARDPIECE_HPP__
#include "SceneNode.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "MySprite.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

/*
 * BoardPiece is an individual tile of the board, texture only
 */
class BoardPiece: public SceneNode
{
public:
    //TILE is a type that cannot effect bullets/tranks movements (empty tile, spawns, flags)
    //WALL is a type that can effect bullets/tranks movements (deflectors, walls, blocks)
    //PARENT is the top level node with no texture.
    enum Type
    {
        PARENT,
        BORDER,
        CORNER,
        TILE,
        DEFLECTOR,
        BLOCK,
        SPAWN_P1,
        SPAWN_P2,
        SPAWN_P3,
        SPAWN_P4,
        FLAG,
        TYPECOUNT
    };
    BoardPiece(Type, TextureHolder*, float, float);
    BoardPiece(Type, TextureHolder*, float, float, float);
    void destroy();
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    float getSpriteWidth();
    float getSpriteHeight();
    int swapChildNode(SceneNode*, int);
    int layerChildNode(SceneNode*, int);



private:
    Type type;
    MySprite sprite;
};

#endif
