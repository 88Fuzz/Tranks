#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "StateStack.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

/*
 * Main application that runs
 */
class Application
{
public:
    Application(int, int, int);
    void run();

private:
    static const sf::Time TIMEPERFRAME;
    sf::RenderWindow window;
    FontHolder fonts;
    TextureHolder textures;
    StateStack stateStack;
    sf::Text statisticsText;
    sf::Time statisticsUpdateTime;
    std::size_t statisticsNumFrames;

    void processInput();
    void update(sf::Time dt);
    void render();
    void updateStatistics(sf::Time dt);
    void registerStates();
};

#endif
