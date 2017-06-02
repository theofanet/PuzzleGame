//
// Created by Theo on 08/11/2016.
//

#include "Entities/Stairs.h"
#include "GGraph/Tile.h"

Stairs::Stairs(GWorld *world, GPoint pos, GSize size) {
    _body = new GRect(pos, size);
    _body->setObjectMode(GPhy::STATIC);
    GObjectData d;
    d.stairs = this;
    _body->setGObjectData(d);
    _body->setDelegate(this);

    world->addObject(_body);
}

Stairs::~Stairs() {

}

void Stairs::draw(Graphics &graphics) {
    if(_tile)
        _tile->draw(graphics);
}