#include "Application.hpp"
#include "StateIdentifiers.hpp"
#include "GameState.hpp"
#include "MainMenuState.hpp"
#include "LobbyState.hpp"
#include <sstream>
#include <iostream>

const sf::Time Application::TIMEPERFRAME = sf::seconds(1.f / 60.f);

Application::Application(int width, int height, int windowStyle) :
                window(sf::VideoMode(width, height), "Tranks", windowStyle),
                textures(),
                fonts(),
                stateStack(State::Context(&window, &textures, &fonts)),
                statisticsText(),
                statisticsUpdateTime(),
                statisticsNumFrames(0)
{
    window.setKeyRepeatEnabled(false);
    fonts.load(Fonts::Sansation, "Media/Sansation.ttf");
    statisticsText.setFont(fonts.get(Fonts::Sansation));
    statisticsText.setPosition(5.f, 5.f);
    statisticsText.setCharacterSize(10u);

    registerStates();
    stateStack.pushState(States::MAIN_MENU);
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while(window.isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        while(timeSinceLastUpdate > TIMEPERFRAME)
        {
            timeSinceLastUpdate -= TIMEPERFRAME;

            processInput();
            update(TIMEPERFRAME);

            // Check inside this loop, because stack might be empty before update() call
            if(stateStack.isEmpty())
                window.close();

        }

        updateStatistics(dt);
        render();
    }
}

void Application::processInput()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        stateStack.handleEvent(&event);

        if(event.type == sf::Event::Closed
                || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            window.close();
    }
}

void Application::update(sf::Time dt)
{
    stateStack.update(dt);
}

void Application::render()
{
    window.clear();

    stateStack.draw();

    window.setView(window.getDefaultView());
    window.draw(statisticsText);

    window.display();
}

/*
 * Displays the FPS in top left corner
 */
void Application::updateStatistics(sf::Time dt)
{
    std::stringstream ss;
    statisticsUpdateTime += dt;
    statisticsNumFrames += 1;
    if(statisticsUpdateTime >= sf::seconds(1.0f))
    {
        ss << "FPS: " << statisticsNumFrames;
        statisticsText.setString(ss.str());

        statisticsUpdateTime -= sf::seconds(1.0f);
        statisticsNumFrames = 0;
    }
}

/*
 * Each game state needs to be registered here before it can be used by the game
 */
void Application::registerStates()
{
    stateStack.registerState<MainMenuState>(States::MAIN_MENU);
    stateStack.registerState<LobbyState>(States::HOST_LOBBY, true);
    stateStack.registerState<LobbyState>(States::JOIN_LOBBY, false);
}
