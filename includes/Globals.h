//
// Created by Theo Fanet on 22/10/2016.
//

#ifndef TESTPROG_GLOBALS_H
#define TESTPROG_GLOBALS_H

#include <iostream>
#include <string>

namespace Globals{
    const int SCREEN_WIDTH  = 640;
    const int SCREEN_HEIGHT = 500;

    const int SPRITE_SCALE = 2;
}

enum Direction{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

struct Vector2{
    Vector2() : x(0), y(0){}
    Vector2(int x, int y) : x(x), y(y){}

    int x, y;
};

namespace Sides{
    enum Side{
        TOP,
        BOTTOM,
        RIGHT,
        LEFT,
        NONE
    };

    inline Side getOppositeSide(Side side){
        return
            side == TOP ? BOTTOM :
            side == BOTTOM ? TOP :
            side == RIGHT ? LEFT :
            side == LEFT ? RIGHT :
            NONE;
    }
}

template<typename T>
void delete_obj(T *&ptr) {
    if(ptr)
        delete ptr;
    ptr = NULL;
}

#endif //TESTPROG_GLOBALS_H
