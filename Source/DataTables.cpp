#include "DataTables.hpp"
#include "BoardPiece.hpp"
#include "MapCreator.hpp"
#include "ButtonTypes.hpp"
#include <stdlib.h>
#include <iostream>

// For std::bind() placeholders _1, _2, ...
//using namespace std::placeholders;

/*
 * Initializes array of TextureData for BoardPiece information.
 * Remember to free returned array!
 */
TextureData *initializeBoardPieceData()
{
    TextureData *data = (TextureData *) malloc(sizeof(TextureData) * BoardPiece::Type::TYPECOUNT);

    data[BoardPiece::Type::PARENT].textureId = Textures::BOARD;
    data[BoardPiece::Type::PARENT].textureRect = sf::IntRect(0, 0, 0, 0);

    data[BoardPiece::Type::BORDER].textureId = Textures::BOARD;
    data[BoardPiece::Type::BORDER].textureRect = sf::IntRect(600, 0, 75, 75);

    data[BoardPiece::Type::CORNER].textureId = Textures::BOARD;
    data[BoardPiece::Type::CORNER].textureRect = sf::IntRect(675, 0, 75, 75);

    data[BoardPiece::Type::TILE].textureId = Textures::BOARD;
    data[BoardPiece::Type::TILE].textureRect = sf::IntRect(0, 0, 75, 75);

    data[BoardPiece::Type::DEFLECTOR].textureId = Textures::BOARD;
    data[BoardPiece::Type::DEFLECTOR].textureRect = sf::IntRect(150, 0, 75, 75);

    data[BoardPiece::Type::BLOCK].textureId = Textures::BOARD;
    data[BoardPiece::Type::BLOCK].textureRect = sf::IntRect(75, 0, 75, 75);

    data[BoardPiece::Type::SPAWN_P1].textureId = Textures::BOARD;
    data[BoardPiece::Type::SPAWN_P1].textureRect = sf::IntRect(300, 0, 75, 75);

    data[BoardPiece::Type::SPAWN_P2].textureId = Textures::BOARD;
    data[BoardPiece::Type::SPAWN_P2].textureRect = sf::IntRect(375, 0, 75, 75);

    data[BoardPiece::Type::SPAWN_P3].textureId = Textures::BOARD;
    data[BoardPiece::Type::SPAWN_P3].textureRect = sf::IntRect(450, 0, 75, 75);

    data[BoardPiece::Type::SPAWN_P4].textureId = Textures::BOARD;
    data[BoardPiece::Type::SPAWN_P4].textureRect = sf::IntRect(525, 0, 75, 75);

    data[BoardPiece::Type::FLAG].textureId = Textures::BOARD;
    data[BoardPiece::Type::FLAG].textureRect = sf::IntRect(225, 0, 75, 75);

    return data;
}

/*
 * Initializes array of TextureData for Player information.
 * Remember to free returned array!
 */
TextureData *initializePlayerData()
{
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
    TextureData *data = (TextureData *) malloc(sizeof(TextureData) * GUI::TRANK_CONTROLS::TRANK_CONTROLS_COUNT);
    int size = 100;
    int j = 0;

    for(int j = 0; j < GUI::TRANK_CONTROLS::TRANK_CONTROLS_COUNT; j++)
    {
        data[j].textureId = Textures::BUTTONS;
        data[j].textureRect = sf::IntRect(j * size, 0, size, 2 * size);
    }
//    data[GUI::TRANK_CONTROLS::MOVE_SINGLE].textureId = Textures::BUTTONS;
//    data[GUI::TRANK_CONTROLS::MOVE_SINGLE].textureRect = sf::IntRect(j++ * size, 0, size, 2 * size);
//
//    data[GUI::TRANK_CONTROLS::MOVE_DOUBLE].textureId = Textures::BUTTONS;
//    data[GUI::TRANK_CONTROLS::MOVE_DOUBLE].textureRect = sf::IntRect(j++ * size, 0, size, 2 * size);
//
//    data[GUI::TRANK_CONTROLS::ROTATE_HALF_CLOCKWISE].textureId = Textures::BUTTONS;
//    data[GUI::TRANK_CONTROLS::ROTATE_HALF_CLOCKWISE].textureRect = sf::IntRect(j++ * size, 0, size, 2 * size);
//
//    data[GUI::TRANK_CONTROLS::ROTATE_HALF_COUNTER].textureId = Textures::BUTTONS;
//    data[GUI::TRANK_CONTROLS::ROTATE_HALF_COUNTER].textureRect = sf::IntRect(j++ * size, 0, size, 2 * size);
//
//    data[GUI::TRANK_CONTROLS::ROTATE_FULL].textureId = Textures::BUTTONS;
//    data[GUI::TRANK_CONTROLS::ROTATE_FULL].textureRect = sf::IntRect(j++ * size, 0, size, 2 * size);
//
//    data[GUI::TRANK_CONTROLS::FIRE].textureId = Textures::BUTTONS;
//    data[GUI::TRANK_CONTROLS::FIRE].textureRect = sf::IntRect(j++ * size, 0, size, 2 * size);

    return data;
}
