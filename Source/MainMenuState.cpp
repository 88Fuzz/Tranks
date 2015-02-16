#include "MainMenuState.hpp"
#include "Button.hpp"
#include "ButtonTypes.hpp"
#include "ResourceHolder.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

MainMenuState::MainMenuState(StateStack *stack, Context context) :
        State(stack, context), buttons()
{
    loadTextures();
    GUI::Button *tmpButton = new GUI::Button(context, GUI::ButtonTypes::BIG_BUTTON, 100, 400, 300, 100);
    tmpButton->setText("HOST");
    buttons.pack(tmpButton);
    tmpButton->setCallback([&] ()
        {
            requestStackSwap(States::HOST_LOBBY);
        });
    tmpButton = new GUI::Button(context, GUI::ButtonTypes::BIG_BUTTON, 100, 500, 300, 100);
    tmpButton->setText("JOIN");
    tmpButton->setCallback([&] ()
        {
            requestStackSwap(States::JOIN_LOBBY);
        });
    buttons.pack(tmpButton);
    tmpButton = new GUI::Button(context, GUI::ButtonTypes::BIG_BUTTON, 100, 600, 300, 100);
    tmpButton->setText("EXIT");
    tmpButton->setCallback([&] ()
        {
            requestStackPop();
        });
    buttons.pack(tmpButton);
}

MainMenuState::~MainMenuState()
{
//    buttons.free();
}

void MainMenuState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.setView(window.getDefaultView());

    window.draw(buttons);
}

/*
 * Updates the world, if return false updates to the gameStack will stop.
 * Useful for a pause menu, will not update the background (will not make
 * a difference if pause menu is not transparent)
 */
bool MainMenuState::update(sf::Time dt)
{
    return true;
}

/*
 * Handle SFML events, ex: mouse clicks, keyboard key presses
 */
bool MainMenuState::handleEvent(const sf::Event* event)
{
    GUI::Component* selected;
    if(event->type == sf::Event::MouseButtonPressed)
    {
        buttons.handleEvent(event);
    }
    else if(event->type == sf::Event::MouseButtonReleased)
    {
        if((selected = buttons.getSelectedComponent()) != NULL)
            ((GUI::Button*) selected)->trigger();
    }

    return true;
}

void MainMenuState::loadTextures()
{
    TextureHolder* textures = getContext().textures;
    textures->load(Textures::BUTTONS, "Media/Textures/Buttons.png");
}
