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
        PARENT,
        BORDER,
        CORNER,
        TILE,
        DEFLECTOR,
        BLOCK,
        SPAWN_P1,
        SPAWN_P2,
        SPAWN_P3,
        SPAWN_P4,
        FLAG,
        BOARD_TYPECOUNT,
        PLAYER,
        NONE
    };
}

#endif
