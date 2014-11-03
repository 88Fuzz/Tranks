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
    BoardPiece(Category::Type, TextureHolder*, float, float);
    BoardPiece(Category::Type, TextureHolder*, float, float, float);
    void destroy();
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    float getSpriteWidth();
    float getSpriteHeight();
    int swapChildNode(SceneNode*, int);
    int layerChildNode(SceneNode*, int);
    bool checkTile(int, Category::Type);

private:
    MySprite sprite;
};

#endif
