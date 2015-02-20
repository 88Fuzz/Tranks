#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "State.hpp"
#include "MySprite.hpp"
#include "DataTables.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <string>

namespace GUI
{
    class Button: public Component
    {
    public:
        typedef std::function<void()> Callback;

        enum ButtonState
        {
            NORMAL,
            PRESSED,
            BUTTON_COUNT
        };

        Button(State::Context, int, int, int, int, int);
        Button(State::Context, int, int, int, int, int, bool);

        void setCallback(Callback);
        void setText(const std::string);
        void setToggle(bool);

        virtual bool isSelectable() const;
        virtual void select();
        virtual void deselect();

        virtual void activate();
        virtual void deactivate();
        virtual bool isDeselectable();

        void trigger();

        virtual bool handleEvent(const sf::Event*);

    private:
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
        void changeTexture(ButtonState buttonButtonState);
        bool checkMouseClickLocation(sf::Vector2f, sf::Vector2i);
        void centerText();

        Callback callbackFunc;
        MySprite sprite;
        sf::Text text;
        bool toggle;
        int buttonWidth;
        int buttonHeight;

        bool deselectable;
        //Used to keep track of original y coordinate of button in texture sheet so that the texture can change state correctly
        int yOffset;
        //Used to get mouse position
        sf::RenderWindow *window;
        sf::IntRect textureRect;
    };
}

#endif
