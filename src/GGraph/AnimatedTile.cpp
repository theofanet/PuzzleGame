//
// Created by Theo Fanet on 23/10/2016.
//


#include <GGraph/animatedTile.h>
#include <GGraph/Graphics.h>

AnimatedTile::AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, SDL_Texture *tileset, Vector2 size,
                           Vector2 position) : Tile(tileset, size, tilesetPositions[0], position){
    _tilesetPosition = tilesetPositions;
    _duration        = duration;
    _tileToDraw      = 0;
}

AnimatedTile::~AnimatedTile() {

}

void AnimatedTile::update(float elapsedTime) {
    if(_amountOfTime <= 0){
        if(_tileToDraw == _tilesetPosition.size() - 1)
            _tileToDraw = 0;
        else
            _tileToDraw++;

        _amountOfTime = _duration;
    }
    else
        _amountOfTime -= elapsedTime;

    Tile::update(elapsedTime);
}

void AnimatedTile::draw(Graphics &graphics) {
    SDL_Rect dstRect = { _position.x, _position.y, _size.x * Globals::SPRITE_SCALE, _size.y * Globals::SPRITE_SCALE };
    SDL_Rect srcRect = { _tilesetPosition[_tileToDraw].x, _tilesetPosition[_tileToDraw].y, _size.x, _size.y };
    graphics.blitSurface(_tileset, &srcRect, &dstRect);
}