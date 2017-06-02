//
// Created by Theo on 06/11/2016.
//

#ifndef PUZZELGAME_SNAKE_H
#define PUZZELGAME_SNAKE_H

#include "Enemy.h"
#include "GGraph/AnimatedSprite.h"

class Graphics;
class Player;

class Snake : public Enemy, public AnimatedSprite, public GObjectDelegate{
public:
    Snake(Graphics &graphics, GWorld *world, GPoint position);
    ~Snake();

    void update(float elapsedTime);
    void draw(Graphics &graphics);

    void setupAnimations();

    inline void setBoundries(float maxX, float minX){
        _maxX = maxX;
        _minX = minX;
    }
    inline float getMaxX(){ return _maxX; }
    inline float getMinX(){ return _minX; }

    void touchActiveTile(GObject *object, Sides::Side side);
    void touchTile(Tile *tile, Sides::Side side);
    void touchPlayer(Player *player, Sides::Side side);

protected:
    float _maxX, _minX;
};


#endif //PUZZELGAME_SNAKE_H
