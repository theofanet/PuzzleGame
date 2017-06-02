//
// Created by Theo Fanet on 23/10/2016.
//

#ifndef TESTPROG_ANIMATEDTILE_H
#define TESTPROG_ANIMATEDTILE_H

#include <vector>

#include <GGraph/Tile.h>
#include <Globals.h>


class AnimatedTile : public Tile{
public:
    AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, SDL_Texture *tileset, Vector2 size, Vector2 position);
    ~AnimatedTile();

    void update(float elapsedTime);
    void draw(Graphics &graphics);

protected:
    int  _amountOfTime = 0;
    bool _notDone      = false;

private:
    std::vector<Vector2> _tilesetPosition;
    int                  _tileToDraw;
    int                  _duration;
};


struct AnimatedTileInfo{
    int TilsetsFirstGid;
    int StartTileId;
    int Duration;

    std::vector<int> TileIds;
};

#endif //TESTPROG_ANIMATEDTILE_H
