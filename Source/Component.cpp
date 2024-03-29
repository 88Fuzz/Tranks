#include "Component.hpp"

namespace GUI
{

    Component::Component() :
            selected(false), active(false), deselectable(true)
    {
    }

    Component::~Component()
    {
    }

    bool Component::isSelected() const
    {
        return selected;
    }

    void Component::select()
    {
        selected = true;
    }

    void Component::deselect()
    {
        selected = false;
    }

    bool Component::isActive() const
    {
        return active;
    }

    void Component::activate()
    {
        active = true;
    }

    void Component::deactivate()
    {
        active = false;
    }

    bool Component::isDeselectable()
    {
        return deselectable;
    }

     void Component::setDeselectable(bool value)
     {
         deselectable = value;
     }
}
