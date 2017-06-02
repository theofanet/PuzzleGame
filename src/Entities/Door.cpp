//
// Created by Theo Fanet on 05/11/2016.
//

#include "Entities/Door.h"


Door::Door(Graphics &graphics, GWorld *world, std::string dest, GPoint pos, GSize size)
    : AnimatedSprite(graphics, "content/sprites/door.png", 0, 0, 32, 32, 0, 0, 100){
    _destination = dest;

    setupAnimations();
    playAnimation("Close");

    _body = new GRect(pos, size);
    _body->setObjectMode(GPhy::STATIC);
    GObjectData d;
    d.door = this;
    _body->setGObjectData(d);
    _body->setDelegate(this);

    world->addObject(_body);
}

Door::~Door() {

}

void Door::setupAnimations() {
    addAnimation(1, 0, 0, "Close", 63, 85, Vector2());
    addAnimation(5, 0, 0, "Open", 63, 85, Vector2());
}

void Door::draw(Graphics &graphics) {
    AnimatedSprite::draw(graphics, _body->getPosition().x, _body->getPosition().y);
}