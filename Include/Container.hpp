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

    public:
        Container();
        virtual ~Container();

        void pack(Component*);

        virtual bool isSelectable() const;
        virtual bool handleEvent(const sf::Event*);

    private:
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

        bool hasSelection() const;
        void select(std::size_t);
        void selectNext();
        void selectPrevious();

    private:
        std::vector<Component*> children;
        //TODO REMOVE THIS
        int selectedChild;
    };

}

#endif
