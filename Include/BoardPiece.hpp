#ifndef __BOARDPIECE_HPP__
#define __BOARDPIECE_HPP__
#include "SceneNode.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "MySprite.hpp"
#include "Moveable.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

/*
 * BoardPiece is an individual tile of the board, texture only
 */
class BoardPiece: public SceneNode
{
public:
    enum DeflectionDirection
    {
        NE = 0,
        SE = 90,
        SW = 180,
        NW = -90
    };
    BoardPiece(Category::Type, TextureHolder*, float, float);
    BoardPiece(Category::Type, TextureHolder*, float, float, float);
    virtual ~BoardPiece();
    virtual void destroy();
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    float getSpriteWidth();
    float getSpriteHeight();
    int swapChildNode(SceneNode*, int);
    int layerChildNode(SceneNode*, int);
    int getDeflection(Moveable::Direction);

private:
    MySprite sprite;
    //0 == coming from NORTH
    //1 == coming from EAST
    //2 == coming from SOUTH
    //3 == coming from WEST
    int deflections[4];

    std::string debug;
};

#endif
