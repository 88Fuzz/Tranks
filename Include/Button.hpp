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

        enum Type
        {
            NORMAL,
            PRESSED,
            BUTTON_COUNT
        };

        Button(State::Context, int, int, int, int, int);

        void setCallback(Callback);
        void setText(const std::string);
        void setToggle(bool);

        virtual bool isSelectable() const;
        virtual void select();
        virtual void deselect();

        virtual void activate();
        virtual void deactivate();

        void trigger();

        virtual bool handleEvent(const sf::Event*);

    private:
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
        void changeTexture(Type buttonType);
        bool checkMouseClickLocation(sf::Vector2f, sf::Vector2i);

        Callback callbackFunc;
        MySprite sprite;
        sf::Text text;
        bool toggle;
        int buttonWidth;
        int buttonHeight;
        //Used to get mouse position
        sf::RenderWindow *window;
        sf::IntRect textureRect;
    };
}

#endif
