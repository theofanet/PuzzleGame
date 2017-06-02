//
// Created by Theo on 06/11/2016.
//

#include "GGraph/ActiveTile.h"
#include "Entities/Player.h"


void ActiveTile::setPhysics(GWorld *world, bool isStatic) {
    Tile::setPhysics(world, isStatic);
    _BBox->setDelegate(this);
    GObjectData d;
    d.activeTile = this;
    _BBox->setGObjectData(d);

    _isPushed = false;
}

void ActiveTile::update(float elapsedTime) {
    Tile::update(elapsedTime);
    //if(!_BBox->isColliding())
    //_BBox->setPushed();
}

void ActiveTile::touchActiveTile(ActiveTile *tile, Sides::Side side) {
    tile->getBBox()->setGrounded(false);

    switch (side) {
        case Sides::BOTTOM:
            _BBox->setPositionY(tile->getBBox()->getPosition().y - _BBox->getBBox().size.h);
            _BBox->setVelocityY(0);
            _BBox->setVelocityX(0);
            break;
        case Sides::LEFT:
            tile->setPushed(_isPushed);
            if(tile->getBBox()->isBlockedLeft()){
                _BBox->setBlockedLeft(true);
                _BBox->setPositionX(tile->getBBox()->getPosition().x + _BBox->getBBox().size.w);
            }
            else{
                //tile->getBBox()->setPositionX(_BBox->getPosition().x - _BBox->getBBox().size.w);
                tile->getBBox()->applyImpulse(GVector(-1, 0));
                tile->setPushed(true);
            }
            break;
        case Sides::RIGHT:
            tile->setPushed(_isPushed);
            if(tile->getBBox()->isBlockedRight()){
                _BBox->setBlockedRight(true);
                _BBox->setPositionX(tile->getBBox()->getPosition().x - _BBox->getBBox().size.w);
            }
            else{
                tile->getBBox()->applyImpulse(GVector(1, 0));
                //tile->getBBox()->setPositionX(_BBox->getPosition().x + _BBox->getBBox().size.w);
                tile->setPushed(true);
            }
            break;
        default:
            break;
    }
}

void ActiveTile::touchTile(Tile *tile, Sides::Side side) {
    switch(side){
        case Sides::TOP:
            _BBox->setPositionY(tile->getBBox()->getPosition().y + tile->getBBox()->getBBox().size.h);
            _BBox->setVelocityY(0);
            break;
        case Sides::BOTTOM:
            _BBox->setPositionY(tile->getBBox()->getPosition().y - _BBox->getBBox().size.h);
            _BBox->setGrounded(true);
            break;
        case Sides::LEFT:
            _BBox->setPositionX(tile->getBBox()->getPosition().x + tile->getBBox()->getBBox().size.w);
            //if(_BBox->isGrounded()){
                //std::cout << "BLOCKED" << std::endl;
                _BBox->setBlockedLeft(true);
            //}
            break;
        case Sides::RIGHT:
            _BBox->setPositionX(tile->getBBox()->getPosition().x - _BBox->getBBox().size.w);
            //if(_BBox->isGrounded())
                _BBox->setBlockedRight(true);
            break;
        default:
            break;
    }
}