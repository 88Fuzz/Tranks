#ifndef __MAINMENUSTATE_HPP__
#define __MAINMENUSTATE_HPP__

#include "State.hpp"
#include "Container.hpp"

class MainMenuState: public State
{
public:
    MainMenuState(StateStack *, Context );

    virtual ~MainMenuState();
    virtual void draw();
    virtual bool update(sf::Time);
    virtual bool handleEvent(const sf::Event *);

private:
    GUI::Container buttons;

    void loadTextures();
};

#endif
