//
// Created by Theo Fanet on 22/10/2016.
//

#ifndef TESTPROG_SPRITE_H
#define TESTPROG_SPRITE_H

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <Globals.h>

class GRect;
class Graphics;

class Sprite{
public:
    Sprite();
    Sprite(Graphics &graphics, const std::string &filePath, int srcX, int srcY, int w, int h, float posX, float posY);
    virtual ~Sprite();

    virtual void update();
    virtual void draw(Graphics &graphics, int x, int y);

    GRect* getBoundingBox() { return _boundingBox; }

    const inline float getX() const {return _x; }
    const inline float getY() const {return _y; }
    inline void setX(float x){ _x = x; }
    inline void setY(float y){ _y = y; }

    void setSourceRectX(int x);
    void setSourceRectY(int y);
    void setSourceRectW(int w);
    void setSourceRectH(int h);

protected:
    SDL_Rect _srcRect;

    SDL_Texture* _spriteSheet;
    SDL_Surface* _surface;

    GRect *_boundingBox;

    float _x, _y;
};

#endif //TESTPROG_SPRITE_H
