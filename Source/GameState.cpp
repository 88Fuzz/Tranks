#include "State.hpp"
#include "GameState.hpp"
#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(StateStack* stack, Context context) :
        State(stack, context), world(context.window, context.fonts)
//, player(*context.player)
{
//	player.setMissionStatus(Player::MissionRunning);
}

void GameState::draw()
{
    world.draw();
}

/*
 * Updates the world, if return false updates to the gameStack will stop.
 * Useful for a pause menu, will not update the background (will not make
 * a difference if pause menu is not transparent)
 */
bool GameState::update(sf::Time dt)
{
    world.update(dt);

//	if (!world.hasAlivePlayer())
//	{
//		player.setMissionStatus(Player::MissionFailure);
//		requestStackPush(States::GameOver);
//	}
//	else if (world.hasPlayerReachedEnd())
//	{
//		player.setMissionStatus(Player::MissionSuccess);
//		requestStackPush(States::GameOver);
//	}
//
//	CommandQueue& commands = world.getCommandQueue();
//	player.handleRealtimeInput(commands);

    return true;
}

/*
 * Handle SFML events, ex: mouse clicks, keyboard key presses
 */
bool GameState::handleEvent(const sf::Event* event)
{
    // Game input handling
//	CommandQueue& commands = world.getCommandQueue();
//	player.handleEvent(event, commands);

    world.handleEvent(event);

// Escape pressed, trigger the pause screen
//	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
//		requestStackPush(States::Pause);

    return true;
}
