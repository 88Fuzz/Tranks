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
        std::cout << "starting game with width " << width << " height " << height << "\n";
        Application *game = new Application(width, height, windowStyle);
        game->run();
        delete game;
    } catch(std::exception& e)
    {
        std::cout << "\nEXCEPTION: " << e.what() << std::endl;
    }

    /*  sf::RenderWindow window(sf::VideoMode(1920, 1080), "Audio", sf::Style::Fullscreen);
     sf::Texture texture;
     if(!texture.loadFromFile("Media/Textures/Board.png"))
     {
     std::cerr << "Could not open texture file\n";
     return -1;
     }

     window.setKeyRepeatEnabled(false);
     window.setVerticalSyncEnabled(true);

     // Create and load resource
     //    std::unique_ptr<Resource> resource(new Resource());
     //    if (!resource->loadFromFile(filename))
     //        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
     //
     // If loading successful, insert resource to map
     //    insertResource(id, std::move(resource));

     TextureHolder textureHolder;
     textureHolder.load(Textures::ID::BOARD, "Media/Textures/Board.png");

     MapCreator mc(&textureHolder);

     mc.generate("Media/Maps/test.xml");
     BoardPiece *board = mc.getMap();

     sf::RenderTexture teexture;
     teexture.create(mc.getWidth() * 75, mc.getHeight() * 75);

     teexture.draw(*board);
     teexture.display();

     sf::Sprite *sprite9 = new sf::Sprite(teexture.getTexture());
     sprite9->setPosition(0, 0);

     while(window.isOpen())
     {
     sf::Event event;
     while(window.pollEvent(event))
     {
     if(event.type == sf::Event::MouseWheelMoved)
     {
     std::cout << "movement " << event.mouseWheel.delta << "\n";
     std::cout << "mouse x: " << sf::Mouse::getPosition(window).x << " y: "
     << sf::Mouse::getPosition(window).y << "\n";
     }
     if(event.type == sf::Event::Closed)
     window.close();

     if(event.type == sf::Event::Closed
     || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
     window.close();
     }

     sprite9->move(1, 0);
     window.clear(sf::Color::Yellow);
     window.draw(*sprite9);
     //        sf::IntRect *rect = new sf::IntRect(0, 0, 75, 75);
     //        sf::Sprite *sprite = new sf::Sprite(texture, *rect);
     //        sprite->setPosition(0, 10);
     //        window.draw(*sprite);
     //        delete(rect);
     //
     //        rect = new sf::IntRect(75, 0, 75, 75);
     //        sprite = new sf::Sprite(texture, *rect);
     //        sprite->setPosition(75, 10);
     //        window.draw(*sprite);
     //        delete(rect);
     //        delete(sprite);

     //        rect = new sf::IntRect(150, 0, 75, 75);
     //        sprite = new sf::Sprite(texture, *rect);
     //        sprite->setPosition(150, 10);
     //        window.draw(*sprite);
     //
     //        std::cout << "x: " << sprite->getPosition().x << " y: " << sprite->getPosition().y << std::endl;

     //        sprite->rotate(90);
     //        window.draw(*sprite);
     //        std::cout << "rotated x: " << sprite->getPosition().x << " y: " << sprite->getPosition().y << std::endl;

     //        sprite->setPosition(150, 160);
     //        sprite->rotate(90);
     //        window.draw(*sprite);
     //
     //        sprite->setPosition(150, 235);
     //        sprite->rotate(90);
     //        window.draw(*sprite);

     //        delete (rect);
     //        delete (sprite);

     //        rect = new sf::IntRect(150, 0, 75, 75);
     //        sprite = new sf::Sprite(texture, *rect);
     //        sprite->setPosition(150, 10);
     //        window.draw(*sprite);
     //        delete(rect);
     //        delete(sprite);
     //
     //        rect = new sf::IntRect(225, 0, 75, 75);
     //        sprite = new sf::Sprite(texture, *rect);
     //        sprite->setPosition(225, 10);
     //        window.draw(*sprite);
     //        delete(rect);
     //        delete(sprite);
     //
     //        rect = new sf::IntRect(300, 0, 75, 75);
     //        sprite = new sf::Sprite(texture, *rect);
     //        sprite->setPosition(300, 10);
     //        window.draw(*sprite);
     //        delete(rect);
     //        delete(sprite);
     //
     //        rect = new sf::IntRect(375, 0, 75, 75);
     //        sprite = new sf::Sprite(texture, *rect);
     //        sprite->setPosition(375, 10);
     //        window.draw(*sprite);
     //        delete(rect);
     //        delete(sprite);
     //
     //        rect = new sf::IntRect(450, 0, 75, 75);
     //        sprite = new sf::Sprite(texture, *rect);
     //        sprite->setPosition(450, 10);
     //        window.draw(*sprite);
     //        delete(rect);
     //        delete(sprite);
     //
     //        rect = new sf::IntRect(525, 0, 75, 75);
     //        sprite = new sf::Sprite(texture, *rect);
     //        sprite->setPosition(525, 10);
     //        window.draw(*sprite);
     //        delete(rect);
     //        delete(sprite);

     window.display();
     }
     */
    return 0;
}
