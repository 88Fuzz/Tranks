#include "Container.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

namespace GUI
{

    Container::Container() :
            children(), selectedChild(-1)
    {
    }

    Container::~Container()
    {
        for(std::vector<Component *>::iterator it = children.begin(); it != children.end(); it++)
            delete *it;
    }

    void Container::free()
    {
        for(std::vector<Component *>::iterator it = children.begin(); it != children.end(); it++)
            delete *it;

    }

    void Container::pack(Component* component)
    {
        children.push_back(component);
    }

    bool Container::isSelectable() const
    {
        return false;
    }

    /*
     * returns the component that is selected. Returns NULL if nothing is selected
     */
    Component* Container::getSelectedComponent()
    {
        if(selectedChild >= 0)
            return children[selectedChild];
        return NULL;
    }

    /*
     * If an item in the container is selected, then deselect it
     */
    void Container::deselect()
    {
        if(selectedChild >= 0)
            children[selectedChild]->deselect();

        selectedChild = -1;
    }

    bool Container::handleEvent(const sf::Event* event)
    {
        if(event->mouseButton.button == sf::Mouse::Left)
        {
            //On mouse click, set the button as pressed.
            if(event->type == sf::Event::MouseButtonPressed)
            {
                for(int j = 0; j < children.size(); j++)
                {
                    if(children[j]->handleEvent(event))
                    {
                        if(selectedChild >= 0 && children[selectedChild]->isDeselectable())
                            children[selectedChild]->deselect();

                        //If child is selected, set selectedChild, else reset to default
                        if(children[j]->isSelected())
                            selectedChild = j;
                        else
                            selectedChild = -1;

                        return true;
                    }
                }
            }
        }
        return false;
    }

    void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        for(std::vector<Component *>::const_iterator it = children.begin(); it != children.end(); it++)
            target.draw(*(*it), states);
    }

    bool Container::hasSelection() const
    {
        return selectedChild >= 0;
    }

    void Container::select(std::size_t index)
    {
        if(children[index]->isSelectable())
        {
            if(hasSelection())
                children[selectedChild]->deselect();

            children[index]->select();
            selectedChild = index;
        }
    }

    void Container::selectNext()
    {
        if(!hasSelection())
            return;

        // Search next component that is selectable, wrap around if necessary
        int next = selectedChild;
        do
            next = (next + 1) % children.size();
        while(!children[next]->isSelectable());

        // Select that component
        select(next);
    }

    void Container::selectPrevious()
    {
        if(!hasSelection())
            return;

        // Search previous component that is selectable, wrap around if necessary
        int prev = selectedChild;
        do
            prev = (prev + children.size() - 1) % children.size();
        while(!children[prev]->isSelectable());

        // Select that component
        select(prev);
    }

}
