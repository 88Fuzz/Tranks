#ifndef __GAMESTATE_HPP__
#define __GAMESTATE_HPP__

#include "State.hpp"
#include "World.hpp"
//#include "Player.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameState: public State
{
public:
    GameState(StateStack* stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time);
    virtual bool handleEvent(const sf::Event*);

private:
    World world;
//    Player* player;
};

#endif
