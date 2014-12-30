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
    Bullet(int, int, Player::Direction, int, TextureHolder *);
    virtual ~Bullet();

private:
    static constexpr float MOVEMENT_SPEED = 0.26;

    MySprite sprite;
};

#endif
