#include "Bullet.hpp"
#include "DataTables.hpp"

Bullet::Bullet(int posX, int posY, Player::Direction direction, int playerNum, TextureHolder *textures)
{
    TextureData* table = initializePlayerData();
    sprite = MySprite(textures->get(table[playerNum].textureId), table[playerNum].textureRect);
    sprite.centerOrigin();
    sprite.setPosition(0, 0);

    //TODO change to vector. yo
    free(table);
}

Bullet::~Bullet()
{

}
