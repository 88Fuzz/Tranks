#ifndef __MYSPRITE_HPP__
#define __MYSPRITE_HPP__
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

/*
 * Wrapper around the sf::Sprite class that is mainly used for putting the origin at the center (can rotate about center of sprite)
 * and placing the x & y coordinates based on the left corner (move sprite based on top left corner)
 */
class MySprite: public sf::Sprite
{
public:
    MySprite();
    explicit MySprite(const sf::Texture& texture);
    MySprite(const sf::Texture& texture, const sf::IntRect& rectangle);
    void centerOrigin();
    void setBounds();
    void setPosition(float, float);
    void setPosition(const sf::Vector2f&);
    sf::Vector2f getPosition() const;
    float getWidth();
    float getHeight();

private:
    float width;
    float height;

};

#endif /* MYSPRITE_HPP_ */
