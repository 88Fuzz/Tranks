#ifndef __CONTAINER_HPP__
#define __CONTAINER_HPP__

#include "Component.hpp"

#include <vector>
#include <memory>

namespace GUI
{

    class Container: public Component
    {
    public:
        typedef std::shared_ptr<Container> Ptr;

        Container();
        virtual ~Container();

        void pack(Component*);

        virtual bool isSelectable() const;
        virtual bool handleEvent(const sf::Event*);
        Component* getSelectedComponent();
        virtual void deselect();

    private:
        std::vector<Component*> children;
        int selectedChild;

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
        bool hasSelection() const;
        void select(std::size_t);
        void selectNext();
        void selectPrevious();
    };

}

#endif
