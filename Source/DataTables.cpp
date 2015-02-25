#include "DataTables.hpp"
#include "BoardPiece.hpp"
#include "MapCreator.hpp"
#include "ButtonTypes.hpp"
#include <stdlib.h>
#include <iostream>

/*
 * Initializes array of TextureData for BoardPiece information.
 * Remember to free returned array!
 */
TextureData *initializeBoardPieceData()
{
    TextureData *data = (TextureData *) malloc(sizeof(TextureData) * Category::Type::BOARD_TYPECOUNT);

    data[Category::Type::PARENT].textureId = Textures::BOARD;
    data[Category::Type::PARENT].textureRect = sf::IntRect(0, 0, 0, 0);

    data[Category::Type::BORDER].textureId = Textures::BOARD;
    data[Category::Type::BORDER].textureRect = sf::IntRect(600, 0, 75, 75);

    data[Category::Type::CORNER].textureId = Textures::BOARD;
    data[Category::Type::CORNER].textureRect = sf::IntRect(675, 0, 75, 75);

    data[Category::Type::TILE].textureId = Textures::BOARD;
    data[Category::Type::TILE].textureRect = sf::IntRect(0, 0, 75, 75);

    data[Category::Type::DEFLECTOR].textureId = Textures::BOARD;
    data[Category::Type::DEFLECTOR].textureRect = sf::IntRect(150, 0, 75, 75);

    data[Category::Type::BLOCK].textureId = Textures::BOARD;
    data[Category::Type::BLOCK].textureRect = sf::IntRect(75, 0, 75, 75);

    data[Category::Type::SPAWN_P1].textureId = Textures::BOARD;
    data[Category::Type::SPAWN_P1].textureRect = sf::IntRect(300, 0, 75, 75);

    data[Category::Type::SPAWN_P2].textureId = Textures::BOARD;
    data[Category::Type::SPAWN_P2].textureRect = sf::IntRect(375, 0, 75, 75);

    data[Category::Type::SPAWN_P3].textureId = Textures::BOARD;
    data[Category::Type::SPAWN_P3].textureRect = sf::IntRect(450, 0, 75, 75);

    data[Category::Type::SPAWN_P4].textureId = Textures::BOARD;
    data[Category::Type::SPAWN_P4].textureRect = sf::IntRect(525, 0, 75, 75);

    data[Category::Type::FLAG].textureId = Textures::BOARD;
    data[Category::Type::FLAG].textureRect = sf::IntRect(225, 0, 75, 75);

    return data;
}

/*
 * Initializes array of TextureData for Player information.
 * Remember to free returned array!
 */
TextureData *initializePlayerData()
{
    //TODO change PLAYER_COUNT to something better
    TextureData *data = (TextureData *) malloc(sizeof(TextureData) * MapCreator::PLAYER_COUNT);
    int size = 75;

    for(int j = 0; j < MapCreator::PLAYER_COUNT; j++)
    {
        data[j].textureId = Textures::TRANKS;
        data[j].textureRect = sf::IntRect(j * size, 0, size, size);
    }

    return data;
}

/*
 * Initializes array of TextureData for Button information.
 * Remember to free returned array!
 */
TextureData *initializeButtonData()
{
    TextureData *data = (TextureData *) malloc(sizeof(TextureData) * GUI::ButtonTypes::BUTTON_TYPES_COUNT);
    int size = 100;
    int j;

    for(j = 0; j <= GUI::ButtonTypes::CHECK_BOX; j++)
    {
        data[j].textureId = Textures::BUTTONS;
        data[j].textureRect = sf::IntRect(j * size, 0, size, 2 * size);
    }

    data[j].textureId = Textures::BUTTONS;
    data[j].textureRect = sf::IntRect(0, 2 * size, 3 * size, 2 * size);

    return data;
}

/*
 * Initializes array of TextureData for Bullet information.
 * Remember to free returned array!
 */
TextureData *initializeBulletData()
{
    TextureData *data = (TextureData *) malloc(sizeof(TextureData) * MapCreator::PLAYER_COUNT);
    int size = 20;

    for(int j = 0; j < MapCreator::PLAYER_COUNT; j++)
    {
        data[j].textureId = Textures::BULLETS;
        data[j].textureRect = sf::IntRect(j * size, 0, size, size);
    }
    return data;
}
