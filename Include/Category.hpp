#ifndef __CATEGORY_HPP__
#define __CATEGORY_HPP__

/*
 * Categories of different entities in the game
 */
namespace Category
{
    //Bitshifting could be use to determine type easier
    enum Type
    {
        NONE,
        BOARD,
        PICKUP,
        TANK,
        PROJECTILE
    };
}

#endif
