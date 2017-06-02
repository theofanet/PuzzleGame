//
// Created by Theo Fanet on 22/10/2016.
//

#include <GGraph/Tile.h>
#include <GGraph/Graphics.h>


Tile::Tile() {

}

Tile::Tile(SDL_Texture *tilset, Vector2 size, Vector2 tilsetPosition, Vector2 position) {
    _tileset = tilset;
    _size = size;
    _tilesetPosition = tilsetPosition;
    _position = Vector2(position.x * Globals::SPRITE_SCALE, position.y * Globals::SPRITE_SCALE);
    _BBox = NULL;
}

Tile::~Tile() {

}

void Tile::update(float) {
    if(_BBox){
        _position.x = (int)_BBox->getPosition().x;
        _position.y = (int)_BBox->getPosition().y;
    }
}

void Tile::draw(Graphics &graphics) {
    SDL_Rect dstRect = { _position.x, _position.y, _size.x * Globals::SPRITE_SCALE, _size.y * Globals::SPRITE_SCALE };
    SDL_Rect srcRect = { _tilesetPosition.x, _tilesetPosition.y, _size.x, _size.y };

    graphics.blitSurface(_tileset, &srcRect, &dstRect);
}

void Tile::setPhysics(GWorld *world, bool isStatic) {
    if(!_BBox){
        _BBox = new GRect(GPoint(_position.x, _position.y), GSize(_size.x, _size.y) * Globals::SPRITE_SCALE);
        GObjectData d;
        d.tile = this;
        _BBox->setGObjectData(d);
        if(isStatic)
            _BBox->setObjectMode(GPhy::STATIC);
        world->addObject(_BBox);
    }
}