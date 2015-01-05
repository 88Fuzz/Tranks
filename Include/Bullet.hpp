#ifndef __BULLET_HPP__
#define __BULLET_HPP__
//TODO this may cause a circular dependency with player
#include "Player.hpp"

/*
 * Projectiles that the players fire
 */
class Bullet: public SceneNode
{
public:
    Bullet(sf::Vector2i, int, Player::Direction, int, TextureHolder *);
    virtual ~Bullet();
    void setSpawnPos(sf::Vector2i, Player::Direction);
    virtual void drawCurrent(sf::RenderTarget&, sf::RenderStates) const;
    virtual void updateCurrent(sf::Time);

private:
    static constexpr float MOVEMENT_SPEED = 0.26;
    Player::Direction getForwardDirection();

    MySprite sprite;
    int tileWidth;
    Player::Direction forward;
    sf::Vector2i tilePos;
};

#endif
