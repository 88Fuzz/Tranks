#include <MySprite.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

MySprite::MySprite() :
        Sprite()
{
    width = 0;
    height = 0;
}

MySprite::MySprite(const sf::Texture& texture) :
        Sprite(texture)
{
    setBounds();
}
MySprite::MySprite(const sf::Texture& texture, const sf::IntRect& rectangle) :
        Sprite(texture, rectangle)
{
    setBounds();
}

/*
 * When the origin is set to the middle of the sprite, setPosition will also be based on the center.
 * This method sets the position based on the top left of the sprite
 */
void MySprite::setPosition(float x, float y)
{
    sf::Transformable::setPosition(x + width / 2.0, y + height / 2.0);
}

void MySprite::setPosition(const sf::Vector2f& position)
{
    setPosition(position.x, position.y);
}

/*
 * When the origin is set to the middle of the sprite, getPosition will also be based on the center.
 * This method gets the position based on the top left of the sprite
 */
sf::Vector2f MySprite::getPosition() const
{
    sf::Vector2f position = sf::Transformable::getPosition();
    position.x -= width / 2.0;
    position.y -= height / 2.0;
    return position;
}

/*
 * sets the origin to the center of the sprite, width and height need to be initialized first
 */
void MySprite::centerOrigin()
{
    if(width == 0 || height == 0)
        //TODO LOGGING HERE
        std::cerr << "width and height may not have been initialized\n";

    setOrigin(width / 2.0, height / 2.0);
}

/*
 * gets the width and height of the sprite
 */
void MySprite::setBounds()
{
    sf::FloatRect bounds = getLocalBounds();
    width = bounds.width;
    height = bounds.height;
}

//TODO figure out if there is a better way to do this
float MySprite::getWidth()
{
    return width;
}

//TODO figure out if there is a better way to do this
float MySprite::getHeight()
{
    return height;
}
