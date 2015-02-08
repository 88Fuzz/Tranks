#include "LobbyState.hpp"
#include "Button.hpp"
#include "ButtonTypes.hpp"
#include "ResourceHolder.hpp"
#include <stdlib.h>

LobbyState::LobbyState(StateStack *stack, Context context, bool isHost) :
        State(stack, context), buttons(), isHost(isHost)
{
    loadTextures();

    GUI::Button *tmpButton = new GUI::Button(context, GUI::ButtonTypes::BIG_BUTTON, 100, 400, 300, 100);
    tmpButton->setText("EXIT");
    tmpButton->setCallback([&] ()
    {
        requestStackPop();
    });
    buttons.pack(tmpButton);

    if(isHost)
    {
        tmpButton = new GUI::Button(context, GUI::ButtonTypes::BIG_BUTTON, 100, 300, 300, 100);
        tmpButton->setText("START");
        tmpButton->setCallback([&] ()
        {
            requestStackSwap(States::GAME);
        });
        buttons.pack(tmpButton);
    }
}

LobbyState::~LobbyState() { }

void LobbyState::draw()
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
bool LobbyState::update(sf::Time dt)
{
    return true;
}

/*
 * Handle SFML events, ex: mouse clicks, keyboard key presses
 */
bool LobbyState::handleEvent(const sf::Event *event)
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

void LobbyState::loadTextures()
{
    TextureHolder* textures = getContext().textures;
    textures->load(Textures::BUTTONS, "Media/Textures/Buttons.png");
}
