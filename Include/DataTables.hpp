#ifndef __DATATABLES_HPP__
#define __DATATABLES_HPP__

#include "ResourceIdentifiers.hpp"

/*
 * Used to initialize textures for the BoardPiece and Player class
 */
struct TextureData
{
    //Key for the texture to use
    Textures::ID textureId;
    //The rectangle inside the texture to use
    sf::IntRect textureRect;
};


TextureData *initializeBoardPieceData();
TextureData *initializePlayerData();
TextureData *initializeButtonData();
TextureData *initializeBulletData();

#endif
