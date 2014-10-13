#include "Button.hpp"
#include "ResourceHolder.hpp"
#include "ButtonTypes.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

namespace GUI
{

    Button::Button(State::Context context, int enumButton, int x, int y, int width, int height) :
                    callbackFunc(),
                    text("", context.fonts->get(Fonts::Sansation), 16),
                    window(context.window),
                    toggle(false),
                    buttonWidth(width),
                    buttonHeight(height)
    {
        TextureData* table = initializeButtonData();
        sprite = MySprite(context.textures->get(table[enumButton].textureId), table[enumButton].textureRect);
        sprite.centerOrigin();
        sprite.setPosition(x, y);
        textureRect = table[enumButton].textureRect;

        //TODO change to vector. yo
        free(table);

        changeTexture(NORMAL);

//        sf::FloatRect bounds = sprite.getLocalBounds();
//        text.setPosition(bounds.width / 2.f, bounds.height / 2.f);
    }

    void Button::setCallback(Callback callback)
    {
        callbackFunc = std::move(callback);
    }

    void Button::setText(const std::string text)
    {
        this->text.setString(text);
//	centerOrigin(text);
    }

    void Button::setToggle(bool flag)
    {
        toggle = flag;
    }

    bool Button::isSelectable() const
    {
        return true;
    }

    void Button::select()
    {
        Component::select();

        changeTexture(PRESSED);
    }

    void Button::deselect()
    {
        Component::deselect();

        changeTexture(NORMAL);
    }

    void Button::activate()
    {
        Component::activate();

        // If we are toggle then we should show that the button is pressed and thus "toggled".
        if(toggle)
            changeTexture(PRESSED);

        if(callbackFunc)
            callbackFunc();

        // If we are not a toggle then deactivate the button since we are just momentarily activated.
        if(!toggle)
            deactivate();
    }

    void Button::deactivate()
    {
        Component::deactivate();

        if(toggle)
        {
            // Reset texture to right one depending on if we are selected or not.
            if(isSelected())
                changeTexture(PRESSED);
            else
                changeTexture(NORMAL);
        }
    }

    bool Button::handleEvent(const sf::Event* event)
    {
        sf::Vector2f buttonPos = sprite.getPosition();
        //TODO this will probably break horribly
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

        if(event->type == sf::Event::MouseButtonPressed)
        {
            if(checkMouseClickLocation(buttonPos, mousePos))
            {
                //if mouse clicked on button, check if button is selected. If already selected, unselect
                if(isSelected())
                    deselect();
                else
                    select();
                return true;
            }
        }
        return false;
    }

    bool Button::checkMouseClickLocation(sf::Vector2f buttonPos, sf::Vector2i mousePos)
    {
        //Check if mouse click is inside x button
        if(buttonPos.x < mousePos.x && buttonPos.x + buttonWidth > mousePos.x)
        {
            //Check if mouse click is inside y button
            if(buttonPos.y < mousePos.y && buttonPos.y + buttonHeight > mousePos.y)
            {
                return true;
            }
        }
        return false;
    }

    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(sprite, states);
        target.draw(text, states);
    }

    void Button::changeTexture(Type buttonType)
    {
        sf::IntRect newTextureRect = textureRect;
        newTextureRect.top = buttonHeight * buttonType;
        newTextureRect.width = buttonWidth;
        newTextureRect.height = buttonHeight;

        sprite.setTextureRect(newTextureRect);
    }
}