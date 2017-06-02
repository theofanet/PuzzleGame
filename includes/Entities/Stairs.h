//
// Created by Theo on 08/11/2016.
//

#ifndef PUZZELGAME_STAIRS_H
#define PUZZELGAME_STAIRS_H

#include "GPhy/GWorld.h"

class Graphics;

class Stairs : public GObjectDelegate{
public:
    Stairs(GWorld *world, GPoint pos, GSize size);
    ~Stairs();

    inline GRect* getBody(){ return _body; }

    inline void setTile(Tile *t){ _tile = t; }

    void draw(Graphics &graphics);

protected:
    GRect *_body;
    Tile  *_tile;
};


#endif //PUZZELGAME_STAIRS_H
