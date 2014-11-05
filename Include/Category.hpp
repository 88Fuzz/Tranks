#ifndef __CATEGORY_HPP__
#define __CATEGORY_HPP__

/*
 * Categories of different entities in the game
 */
namespace Category
{
    //Bitshifting could be use to determine type easier
    //TILE is a type that cannot effect bullets/tranks movements (empty tile, spawns, flags)
    //WALL is a type that can effect bullets/tranks movements (deflectors, walls, blocks)
    //PARENT is the top level node with no texture.
    enum Type
    {
        PARENT = 1 << 1,
        BORDER = 1 << 2,
        CORNER = 1 << 3,
        TILE = 1 << 4,
        DEFLECTOR = 1 << 5,
        BLOCK = 1 << 6,
        SPAWN_P1 = 1 << 7,
        SPAWN_P2 = 1 << 8,
        SPAWN_P3 = 1 << 9,
        SPAWN_P4 = 1 << 10,
        FLAG = 1 << 11,
        BOARD_TYPECOUNT = 1 << 12,
        PLAYER = 1 << 13,
        NONE = 1 << 14,
        MOVEMENT_SPACE = BORDER | CORNER | TILE | SPAWN_P1 | SPAWN_P2 | SPAWN_P3 | SPAWN_P4 | FLAG
    };
}

#endif
