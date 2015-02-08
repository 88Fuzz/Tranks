#include "State.hpp"
#include "StateStack.hpp"

State::Context::Context(sf::RenderWindow* window, TextureHolder* textures, FontHolder* fonts) :
        window(window), textures(textures), fonts(fonts)
{
}

State::State(StateStack* stack, Context context) :
        stack(stack), context(context)
{
}

State::~State()
{
}

void State::requestStackSwap(States::ID stateID)
{
    stack->swapState(stateID);
}

void State::requestStackPush(States::ID stateID)
{
    stack->pushState(stateID);
}

void State::requestStackPop()
{
    stack->popState();
}

void State::requestStateClear()
{
    stack->clearStates();
}

State::Context State::getContext() const
{
    return context;
}
