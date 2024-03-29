#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace sf
{
    class Event;
}

namespace GUI
{
    class Component: public sf::Drawable, public sf::Transformable, private sf::NonCopyable
    {
    public:
        Component();
        virtual ~Component();

        virtual bool isSelectable() const = 0;
        bool isSelected() const;
        virtual void select();
        virtual void deselect();

        virtual bool isActive() const;
        virtual void activate();
        virtual void deactivate();
        virtual bool isDeselectable();
        virtual void setDeselectable(bool);

        virtual bool handleEvent(const sf::Event*) = 0;

    private:
        bool deselectable;
        bool selected;
        bool active;
    };
}

#endif
