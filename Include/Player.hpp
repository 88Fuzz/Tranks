#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__
#include "Bullet.hpp"
#include "Moveable.hpp"
#include "SceneNode.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "BoardPiece.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

/*
 * Each Trank is a player.
 */
class Player: public Moveable
{
public:
    enum Rotation
    {
        CLOCKWISE = 1,
        COUNTER_CLOCKWISE = -1
    };
    static const int SINGLE_ROTATION = 90;
    static const int DOUBLE_ROTATION = 180;
    static const int SINGLE_MOVE = 75;
    static const int DOUBLE_MOVE = 150;

    Player(int, TextureHolder*);
    virtual ~Player();
    void respawn();
    void setSpawnPos(sf::Vector2i, Direction);
    int getSpawnPosX();
    int getSpawnPosY();
    sf::Vector2i getTilePos();
    sf::Vector2i getTilePos(int);
    sf::Vector2i getTileCorrectedPos(int);
    void startRotation(Rotation, int);
    void startMovement(int);
    void startFire();
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time);
    bool isActionExecuting();
    void setMap(BoardPiece *);
    virtual bool isAlive();
    void setAlive(bool);
    void setScore(int);
    void addScore(int);
    int getScore();
    int getScore(int);
    int getPlayerNum();
    void incrementDeathClock();
    bool didScore();

private:
    static constexpr float ROTATION_SPEED = 0.13;
    static constexpr float MOVEMENT_SPEED = 0.13;
    const static int RESPAWN_LIMIT = 1;

    //Tile position of spawn
    sf::Vector2i spawnPos;
    //Current tile position, for fine grained position use sprite
    sf::Vector2i tilePos;
    int playerNumber;

    //Direction trank is facing at spawn
    Direction spawnFacing;


    //Rotation variables
    Rotation rotationDir;
    int rotationLimit;
    bool rotating;

    //Movement variables
    int moveLimit;
    bool moving;

    //true if player is moving, rotating, or shooting
    bool actionExecuting;

    bool checkValidMove(int);
    bool alive;

    Bullet *bullet;
    int playerNum;
    int score;

    BoardPiece *map;
    //True if score has updated this round
    bool scored;

    //The amount of turns player has been dead
    int deathRespawnCount;
};

#endif
