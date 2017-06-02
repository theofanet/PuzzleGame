//
// Created by Theo on 06/11/2016.
//

#ifndef PUZZELGAME_ENEMIE_H
#define PUZZELGAME_ENEMIE_H

#include "GPhy/GWorld.h"

class Graphics;


class Enemy{
public:
    Enemy(){
        _body = NULL;
        _direction = Sides::NONE;
        _velocityX = 1;
    };
    virtual ~Enemy(){}

    virtual void update(float elapsedTime) = 0;
    virtual void draw(Graphics &graphics) = 0;

    inline GRect* getBody(){ return _body; }

    inline Sides::Side getDirection(){return _direction;}
    inline void setDirection(Sides::Side direction){
        _direction = direction;

        if(_body) {
            _body->setVelocityX(
                    _velocityX * (_direction == Sides::RIGHT ? 1 : (_direction == Sides::NONE ? 0 : -1))
            );
        }
    }

protected:
    GRect *_body;
    Sides::Side _direction;
    float _velocityX;
};


#endif //PUZZELGAME_ENEMIE_H
