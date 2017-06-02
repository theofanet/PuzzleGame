//
// Created by Theo Fanet on 22/10/2016.
//

#include "GGraph/Sprite.h"
#include "Globals.h"
#include "GGraph/Graphics.h"
#include "GPhy/GRect.h"

Sprite::Sprite() {

}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int srcX, int srcY, int w, int h, float posX,
               float posY) {
    _x = posX;
    _y = posY;

    _srcRect.x = srcX;
    _srcRect.y = srcY;
    _srcRect.w = w;
    _srcRect.h = h;

    _surface     = graphics.loadImage(filePath);
    _spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), _surface);

    _boundingBox = new GRect(GPoint(_x, _y), GSize(w, h) * Globals::SPRITE_SCALE);
}

Sprite::~Sprite() {

}

void Sprite::update() {

}

void Sprite::draw(Graphics &graphics, int x, int y) {
    SDL_Rect dstRect = { x, y, _srcRect.w * Globals::SPRITE_SCALE, _srcRect.h * Globals::SPRITE_SCALE };
    graphics.blitSurface(_spriteSheet, &_srcRect, &dstRect);
}

void Sprite::setSourceRectX(int x) {
    _srcRect.x = x;
}

void Sprite::setSourceRectY(int y) {
    _srcRect.y = y;
}

void Sprite::setSourceRectW(int w) {
    _srcRect.w = w;
}

void Sprite::setSourceRectH(int h) {
    _srcRect.h = h;
}