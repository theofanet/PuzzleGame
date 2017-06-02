//
// Created by Theo Fanet on 05/11/2016.
//

#ifndef PUZZELGAME_DOOR_H
#define PUZZELGAME_DOOR_H

#include <GGraph/AnimatedSprite.h>
#include "GPhy/GWorld.h"

class Door : public AnimatedSprite, public GObjectDelegate {
public:
    Door(){}
    Door(Graphics &graphics, GWorld *world, std::string dest, GPoint pos, GSize size);
    ~Door();

    inline std::string getDestination(){ return _destination; }

    inline GRect* getBody(){ return _body; }

    void setupAnimations();

    void draw(Graphics &graphics);

private:
    std::string _destination;
    GRect *_body;
};


#endif //PUZZELGAME_DOOR_H
