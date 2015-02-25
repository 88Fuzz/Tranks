#include "Application.hpp"
#include "ResourceIdentifiers.hpp"
#include "BoardPiece.hpp"
#include "MapCreator.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <fstream>

void printError()
{
    std::cerr << "graphics-cfg.txt was not found.\n" <<
            "Please populate it with \n" <<
            "Resolution:<width>,<height>\n" <<
            "Windowed|Fullscreen\n";
}

int main()
{
    std::ifstream inputFile("graphics-cfg.txt");
    std::string line;
    std::string split;
    int width = -1;
    int height = -1;
    int colon;
    int comma;
    int windowStyle = sf::Style::Default;

    if(inputFile.is_open())
    {
        while(getline(inputFile, line))
        {
            colon = line.find(":");
            split = line.substr(0, colon);
            if(split.compare("Resolution") == 0)
            {
                comma = line.find(",");
                width = std::stoi(line.substr(colon + 1, comma - 11)); //minus 11 due to the length of Resolution
                height = std::stoi(line.substr(comma + 1, std::string::npos));
            }
            else if(split.compare("Fullscreen") == 0)
            {
                windowStyle = sf::Style::Fullscreen;
            }
        }
    }
    else
    {
        printError();
        return 0;
    }

    if(width == -1 || height == -1)
    {
        printError();
        return 0;
    }

    try
    {
        Application *game = new Application(width, height, windowStyle);
        game->run();
        delete game;
    } catch(std::exception& e)
    {
        std::cout << "\nEXCEPTION: " << e.what() << std::endl;
    }
    return 0;
}
