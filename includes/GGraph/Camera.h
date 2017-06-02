//
// Created by Theo on 07/11/2016.
//

#ifndef PUZZELGAME_CAMERA_H
#define PUZZELGAME_CAMERA_H

#include <SDL2/SDL_rect.h>
#include "GPhy/GPhy.h"
#include "Globals.h"

class Camera {
public:
    Camera(){
        _size = GSize(Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT);
    }
    Camera(GPoint pos);
    ~Camera();

    SDL_Rect getCameraSDLRect();

    inline void setPosition(float x, float y){ _position = GPoint(x, y); }
    inline void setPosition(GPoint p){ _position = p; }
    inline void setPositionX(float x){ _position.x = x; }
    inline void setPositionY(float y){ _position.y = y; }
    inline GPoint getPosition(){ return _position; }

    inline GSize getSize(){ return _size; }

protected:
    GPoint _position;
    GSize  _size;
};


#endif //PUZZELGAME_CAMERA_H
