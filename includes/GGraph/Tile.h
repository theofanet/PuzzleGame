//
// Created by Theo Fanet on 22/10/2016.
//

#ifndef TESTPROG_TILE_H
#define TESTPROG_TILE_H

#include <Globals.h>
#include "GPhy/GWorld.h"

class Graphics;
struct SDL_Texture;

class Tile{
public:
    Tile();
    Tile(SDL_Texture* tilset, Vector2 size, Vector2 tilsetPosition, Vector2 position);
    ~Tile();

    void update(float elapsedTime);
    void draw(Graphics &graphics);

    inline GRect* getBBox(){ return _BBox; }
    virtual void setPhysics(GWorld *world, bool isStatic = true);

protected:
    SDL_Texture* _tileset;

    GRect *_BBox;
    bool _hasPhysics;

    Vector2 _size;
    Vector2 _tilesetPosition;
    Vector2 _position;
};

#endif //TESTPROG_TILE_H
