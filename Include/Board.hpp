#ifndef __BOARD_HPP__
#define __BOARD_HPP__

/*
 * Board class is the overall class with information about the current map
 */
class Board : public SceneNode
{
public:
    Board();
private:
    sf::Vector2i player1Spawn;
    sf::Vector2i player2Spawn;
    sf::Vector2i player3Spawn;
    sf::Vector2i player4Spawn;
    sf::Vector2i dimentions;

};



#endif
