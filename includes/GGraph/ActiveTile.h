//
// Created by Theo on 06/11/2016.
//

#ifndef PUZZELGAME_ACTIVETILE_H
#define PUZZELGAME_ACTIVETILE_H

#include "GGraph/Tile.h"

class Player;


class ActiveTile : public Tile, public GObjectDelegate{
public:
    ActiveTile(){}
    ActiveTile(SDL_Texture* tilset, Vector2 size, Vector2 tilsetPosition, Vector2 position) : Tile(tilset, size, tilsetPosition, position){}

    void setPhysics(GWorld *world, bool isStatic = true);

    void touchActiveTile(ActiveTile *tile, Sides::Side side);
    void touchTile(Tile* tile, Sides::Side side);

    void update(float elapsedTime);

    inline void setPushed(bool p){ _isPushed = p; }
    inline bool isPushed(){ return _isPushed; }
protected:
    bool _isPushed;
};


#endif //PUZZELGAME_ACTIVETILE_H
