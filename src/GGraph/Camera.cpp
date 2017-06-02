//
// Created by Theo on 07/11/2016.
//

#include "GGraph/Camera.h"


Camera::Camera(GPoint pos) {
    _position = pos;
    _size     = GSize(Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT);
}

Camera::~Camera() {

}

SDL_Rect Camera::getCameraSDLRect() {
    SDL_Rect rect = { (int)_position.x, (int)_position.y, (int)_size.w, (int)_size.h };
    return rect;
}