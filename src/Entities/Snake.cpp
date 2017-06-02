//
// Created by Theo on 06/11/2016.
//

#include "Entities/Snake.h"
#include "GGraph/Graphics.h"
#include "GGraph/Tile.h"


Snake::Snake(Graphics &graphics, GWorld *world, GPoint position) : Enemy(), AnimatedSprite(graphics, "content/sprites/characters.png", 0, 0, 32, 32, position.x, position.y, 200){
    _body = new GRect(position, GSize(25, 25));
    _body->setDelegate(this);
    GObjectData d;
    d.enemie = this;
    _body->setGObjectData(d);
    world->addObject(_body);

    _minX = position.x;
    _maxX = position.x;
    _velocityX = 0.7;

    setDirection(Sides::RIGHT);

    setupAnimations();
    playAnimation("Walk");
}

Snake::~Snake() {

}

void Snake::setupAnimations() {
    addAnimation(1, 0, 3, "Idle", 32, 32, Vector2());
    addAnimation(3, 0, 3, "Walk", 32, 32, Vector2());
}

void Snake::draw(Graphics &graphics) {
    AnimatedSprite::draw(graphics, _x, _y, 0, NULL, (_direction == Sides::RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL));
}

void Snake::update(float elapsedTime) {
    setDirection(_direction);

    if(_direction == Sides::RIGHT){
        if(_body->getPosition().x > _maxX){
            _body->setPositionX(_maxX);
            setDirection(Sides::LEFT);
        }
    }
    else if(_direction == Sides::LEFT){
        if(_body->getPosition().x < _minX){
            _body->setPositionX(_minX);
            setDirection(Sides::RIGHT);
        }
    }

    GVector adjustPosition;
    adjustPosition.x = -(_srcRect.w - _body->getSize().w / 2);
    adjustPosition.y = -(_srcRect.h - _body->getSize().h / 2) * 2;

    _x = _body->getPosition().x + adjustPosition.x;
    _y = _body->getPosition().y + adjustPosition.y;

    AnimatedSprite::update(elapsedTime);
}

void Snake::touchActiveTile(GObject *object, Sides::Side side) {
    switch (side) {
        case Sides::BOTTOM:
            _body->setPositionY(object->getPosition().y - _body->getBBox().size.h);
            _body->setVelocityY(0);
            break;
        case Sides::RIGHT:
            if(object->isBlockedLeft()){
                _body->setBlockedLeft(true);
                _body->setPositionX(object->getPosition().x + _body->getBBox().size.w);
            }
            else
                object->setPositionX(_body->getPosition().x - _body->getBBox().size.w);
            break;
        case Sides::LEFT:
            if(object->isBlockedRight()){
                _body->setBlockedRight(true);
                _body->setPositionX(object->getPosition().x - _body->getBBox().size.w);
            }
            else
                object->setPositionX(_body->getPosition().x + _body->getBBox().size.w);
            break;
        default:
            break;
    }
}

void Snake::touchTile(Tile *tile, Sides::Side side) {
    switch(side){
        case Sides::TOP:
            _body->setPositionY(tile->getBBox()->getPosition().y + tile->getBBox()->getBBox().size.h);
            _body->setVelocityY(0);
            break;
        case Sides::BOTTOM:
            _body->setPositionY(tile->getBBox()->getPosition().y - _body->getBBox().size.h);
            _body->setGrounded(true);
            break;
        case Sides::LEFT:
            if(_body->isGrounded()){
                _body->setPositionX(tile->getBBox()->getPosition().x + tile->getBBox()->getBBox().size.w);
                if(_direction == Sides::LEFT && _body->isGrounded())
                    setDirection(Sides::RIGHT);
            }
            break;
        case Sides::RIGHT:
            if(_body->isGrounded()){
                _body->setPositionX(tile->getBBox()->getPosition().x - _body->getBBox().size.w);
                if(_direction == Sides::RIGHT)
                    setDirection(Sides::LEFT);
            }
            break;
        default:
            break;
    }
}