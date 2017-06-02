//
// Created by glab on 11/3/16.
//

#ifndef PUZZELGAME_LEVEL_H
#define PUZZELGAME_LEVEL_H

#include "Globals.h"
#include <map>
#include <GGraph/Tile.h>
#include "GGraph/ActiveTile.h"
#include <GPhy/GRect.h>
#include <vector>
#include <GPhy/GWorld.h>
#include <GGraph/Graphics.h>
#include "GGraph/Font.h"
#include "Entities/Door.h"
#include "Entities/Player.h"
#include "Entities/Snake.h"
#include "Entities/Stairs.h"

class Graphics;
class Player;

typedef std::map<std::pair<int,int>, Tile*> TileList;


struct Tileset{
    SDL_Texture *Texture;
    int firstGid;

    Tileset(){
        firstGid = -1;
    }

    Tileset(SDL_Texture *texture, int firstGid) : Texture(texture), firstGid(firstGid){}
};


class LevelDelegate{
public:
    virtual void playerReachDoor(Door *d){ }
    virtual void resetLevel(Level *level){ }
};


class Level : public GCollisionDelegate, public PlayerDelegate{
public:
    Level(){}
    Level(Graphics &graphics, Player *player, std::string mapName, Vector2 spawnPoint = Vector2());
    ~Level();

    void update(float elapsedTime);
    void draw(Graphics &graphics);

    Vector2 getPlayerSpawnPoint(){return _spawnPoint;}

    void collisionReaction(GObject *o1, GObject *o2);
    void collisionEnded(GObject *o1, GObject *o2);
    void intersectDetected(GObject *o1, GObject *o2, GVector MTD);

    inline void setDelegate(LevelDelegate *delegate){ _delegate = delegate; }

    inline void setDrawDebug(bool d){ _drawDebug = d; }
    inline void toggleDrawDebug(){ _drawDebug = !_drawDebug; }
    inline bool isDrawingDebug(){ return _drawDebug; }

    void playerIsDead(Player *player);

    inline std::string getMapName(){ return _mapName; }

private:
    std::string _mapName;

    Vector2                 _spawnPoint;
    Vector2                 _size;
    Vector2                 _tileSize;
    TileList                _tiles[3];
    std::vector<Tileset>    _tilesets;
    std::vector<Door*>       _doors;
    std::vector<ActiveTile*> _pushingTile;
    std::vector<Stairs*>     _stairs;

    Player *_player;
    GWorld *_world;
    std::vector<Enemy*> _enemies;
    Font *_font;

    bool _drawDebug;
    bool _resetLevel;

    LevelDelegate *_delegate;

    void loadMap(Graphics &graphics);
};

#endif //PUZZELGAME_LEVEL_H
