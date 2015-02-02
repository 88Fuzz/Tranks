#ifndef __BULLET_HPP__
#define __BULLET_HPP__
#include "Moveable.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "BoardPiece.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

/*
 * Projectiles that the players fire
 */
class Bullet: public Moveable
{
public:
    Bullet(int, TextureHolder *);
    virtual ~Bullet();

    void setSpawnPos(sf::Vector2i, Direction);
    virtual void drawCurrent(sf::RenderTarget&, sf::RenderStates) const;
    virtual void updateCurrent(sf::Time);
    void resetBounceTotal();
    virtual bool isAlive();
    void setMap(BoardPiece *);

private:
    static constexpr float MOVEMENT_SPEED = 0.86;
    static constexpr int BOUNCE_LIMIT = 180;

    void rotate(int);
    void checkTileMovement(sf::Vector2i);

    //Rate to move in x direction
    int xFactor;
    //Rate to move in y direction
    int yFactor;

    //Keep track of how many times the bullet has rotated
    int bounceTotal;

    BoardPiece *map;

    //This is a very, very stupid way of doing this
    bool tileDeflectFlag;
    //If tileDeflectFlag is set, increment tileDeflection
    float tileDeflect;
    int deflection;
    int playerNum;
};

#endif
