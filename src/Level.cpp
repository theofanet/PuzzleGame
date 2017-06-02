//
// Created by glab on 11/3/16.
//

#include "Level.h"

#include <tinyxml2.h>
#include <Utils.h>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace tinyxml2;

Level::Level(Graphics &graphics, Player *player, std::string mapName, Vector2 spawnPoint) {
    _mapName    = mapName;
    _spawnPoint = spawnPoint;
    _size       = Vector2();

    _world = new GWorld(9.81);
    _world->setCollisionDelegate(this);

    loadMap(graphics);

    _font = new Font("content/fonts/3Dventure.ttf", 65);

    _player = player;
    _player->getBody()->setPosition(_spawnPoint.x, _spawnPoint.y);
    _player->getBody()->setVelocity(GVector());
    _player->setWorld(_world);
    _player->getBody()->setGrounded(false);
    _player->setPlayerDelegate(this);

    _world->setPlayer(_player->getBody());

    _delegate = NULL;

    _drawDebug = false;
}

Level::~Level() {
    delete_obj(_world);

    for(size_t i = 0; i < _enemies.size(); i++)
        delete_obj(_enemies.at(i));

    for(size_t i = 0; i < _pushingTile.size(); i++)
        delete_obj(_pushingTile.at(i));

    for(size_t i = 0; i < _doors.size(); i++)
        delete_obj(_doors.at(i));

    for(size_t i = 0; i < _stairs.size(); i++)
        delete_obj(_stairs.at(i));

    for(int layer = 0; layer < 3; layer++){
        for(size_t x = 0; x < _size.x; x++){
            for(size_t y = 0; y < _size.y; y++){
                std::pair<int, int> it = std::make_pair(x, y);
                if(_tiles[layer].count(it))
                    delete_obj(_tiles[layer].at(it));
            }
        }
    }
}

/**
 * Updates level
 * @param elapsedTime
 */
void Level::update(float elapsedTime) {
    _world->update(elapsedTime);
    _player->update(elapsedTime);

    for(std::vector<Enemy*>::iterator it = _enemies.begin(); it != _enemies.end(); ++it)
        (*it)->update(elapsedTime);

    for(std::vector<ActiveTile*>::iterator it = _pushingTile.begin(); it != _pushingTile.end(); ++it)
        (*it)->update(elapsedTime);

    for(std::vector<Door*>::iterator it = _doors.begin(); it != _doors.end(); ++it)
        (*it)->update(elapsedTime);
}

/**
 * Draw level
 * @param graphics
 */
void Level::draw(Graphics &graphics) {

    /**
     * Updating the camera
     * - Calculing new position
     * - Check map boundries
     * - Set position
     */
    Camera camera = graphics.getCamera();
    GPoint campos = GPoint(
            _player->getX() + _player->getBody()->getSize().w / 2 - Globals::SCREEN_WIDTH / 2,
            _player->getY() + _player->getBody()->getSize().h / 2 - Globals::SCREEN_HEIGHT / 2
    );

    int mapW = _size.x * _tileSize.x * Globals::SPRITE_SCALE;
    int mapH = _size.y * _tileSize.y * Globals::SPRITE_SCALE;

    if(campos.x < 0)
        campos.x = 0;
    else if(campos.x + camera.getSize().w > mapW)
        campos.x = mapW - camera.getSize().w;

    if(campos.y < 0)
        campos.y = 0;
    else if(campos.y + camera.getSize().h > mapH)
        campos.y = mapH - camera.getSize().h;

    graphics.setCameraPosition(campos);


    /**
     * Drawing level
     * - layer BACK
     * - layer MIDDLE
     * - Stairs
     * - doors
     * - pushing tiles
     * - enemies
     * -  player
     * - layer FRONT
     * - Physics debug
     */
    for(TileList::iterator it = _tiles[0].begin(); it != _tiles[0].end(); ++it)
        (*it).second->draw(graphics);

    for(TileList::iterator it = _tiles[1].begin(); it != _tiles[1].end(); ++it)
        (*it).second->draw(graphics);

    for(std::vector<Stairs*>::iterator it = _stairs.begin(); it != _stairs.end(); ++it)
        (*it)->draw(graphics);

    for(std::vector<Door*>::iterator it = _doors.begin(); it != _doors.end(); ++it)
        (*it)->draw(graphics);

    for(std::vector<ActiveTile*>::iterator it = _pushingTile.begin(); it != _pushingTile.end(); ++it)
        (*it)->draw(graphics);

    for(std::vector<Enemy*>::iterator it = _enemies.begin(); it != _enemies.end(); ++it)
        (*it)->draw(graphics);

    _player->draw(graphics);

    _font->setColor(0,0,0);
    _font->draw(graphics, "Theo Adventure", 60, 10);
    _font->setColorR(255);

    for(TileList::iterator it = _tiles[2].begin(); it != _tiles[2].end(); ++it)
        (*it).second->draw(graphics);

    if(_drawDebug)
        _world->draw(graphics);
}

/**
 * Loads maps from tiled xml file
 * @param graphics
 */
void Level::loadMap(Graphics &graphics) {
    XMLDocument doc;
    std::stringstream ss;

    ss << "content/maps/" << _mapName.c_str() << ".tmx";

    doc.LoadFile(ss.str().c_str());

    XMLElement *mapNode = doc.FirstChildElement("map");

    int width, height;

    mapNode->QueryIntAttribute("width", &width);
    mapNode->QueryIntAttribute("height", &height);
    _size = Vector2(width, height);

    int tileWidth, tileHeight;

    mapNode->QueryIntAttribute("tilewidth", &tileWidth);
    mapNode->QueryIntAttribute("tileheight", &tileHeight);
    _tileSize = Vector2(tileWidth, tileHeight);

    XMLElement *tilesetNode = mapNode->FirstChildElement("tileset");

    while(tilesetNode){
        const char *imageSrc = tilesetNode->FirstChildElement("image")->Attribute("source");
        std::string filePath;
        ss.str(imageSrc);
        ss.clear();
        filePath = ss.str();
        Utils::replace(filePath, "..", "content");
        int firstGid;

        tilesetNode->QueryIntAttribute("firstgid", &firstGid);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(graphics.getRenderer(),graphics.loadImage(filePath));
        _tilesets.push_back(Tileset(texture, firstGid));

        tilesetNode = tilesetNode->NextSiblingElement("tileset");
    }

    XMLElement *layerNode = mapNode->FirstChildElement("layer");

    while(layerNode){
        const char *layerName = layerNode->Attribute("name");
        ss.str(layerName);
        ss.clear();
        int layerIndex = 0;

        if(ss.str() == "back")
            layerIndex = 0;
        else if(ss.str() == "middle")
            layerIndex = 1;
        else if(ss.str() == "front")
            layerIndex = 2;
        else if(ss.str() == "pushing")
            layerIndex = 3;
        else if(ss.str() == "stairs")
            layerIndex = 4;

        XMLElement *dataNode = layerNode->FirstChildElement("data");

        while(dataNode){
            const char *lines = dataNode->GetText();
            ss.str(lines);
            ss.clear();
            int tileCount = 0;
            std::string token;

            while(std::getline(ss, token, ',')){
                //token.erase(std::remove(token.begin(), token.end(), '\n'));

                int gid = atoi(token.c_str());

                if(gid){
                    Tileset tls;
                    int closest = 0;

                    for(std::vector<Tileset>::iterator it = _tilesets.begin(); it != _tilesets.end(); ++it){
                        if((*it).firstGid <= gid){
                            if((*it).firstGid > closest){
                                closest = (*it).firstGid;
                                tls = (*it);
                            }
                        }
                    }

                    if(tls.firstGid != -1){
                        int xx = (tileCount % width)*tileWidth;
                        int yy = tileWidth*(tileCount/width);
                        Vector2 tilePos = Vector2(xx, yy);

                        int tilesetWidth, tilesetHeight;

                        SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);

                        int tsxx = (gid % (tilesetWidth/tileWidth) - 1) * tileWidth;
                        if(tsxx < 0)
                            tsxx = tilesetWidth - tileWidth;
                        int tsyy = ((gid - tls.firstGid)/(tilesetWidth/tileWidth))*tileHeight;

                        Vector2 tilesetPos = Vector2(tsxx, tsyy);

                        if(layerIndex == 3){
                            ActiveTile *tile = new ActiveTile(tls.Texture, Vector2(tileWidth, tileHeight), tilesetPos, tilePos);
                            tile->setPhysics(_world, false);
                            _pushingTile.push_back(tile);
                        }
                        else{
                            Tile *tile = new Tile(tls.Texture, Vector2(tileWidth, tileHeight), tilesetPos, tilePos);

                            if(layerIndex == 4){
                                Stairs *stairs = new Stairs(_world, GPoint(tilePos.x, tilePos.y) * Globals::SPRITE_SCALE, GSize(tileWidth, tileHeight) * Globals::SPRITE_SCALE);
                                stairs->setTile(tile);
                                _stairs.push_back(stairs);
                            }
                            else if(layerIndex == 1)
                                tile->setPhysics(_world);

                            if(layerIndex < 3)
                                _tiles[layerIndex][std::make_pair(xx / tileWidth, yy / tileHeight)] = tile;
                        }
                    }
                }

                tileCount++;
            }

            dataNode = dataNode->NextSiblingElement("data");
        }

        layerNode = layerNode->NextSiblingElement("layer");
    }

    XMLElement *objectgroupNode = mapNode->FirstChildElement("objectgroup");
    while(objectgroupNode){
        const char *objectgroupName = objectgroupNode->Attribute("name");

        ss.str(objectgroupName);
        ss.clear();

        if(ss.str() == "spawn points"){
            XMLElement *objectNode = objectgroupNode->FirstChildElement("object");
            float spx, spy;
            objectNode->QueryFloatAttribute("x", &spx);
            objectNode->QueryFloatAttribute("y", &spy);

            _spawnPoint = Vector2((int)spx * Globals::SPRITE_SCALE, (int)spy * Globals::SPRITE_SCALE);
        }
        else if(ss.str() == "doors"){
            XMLElement *pObject = objectgroupNode->FirstChildElement("object");
            while(pObject){
                float x = pObject->FloatAttribute("x");
                float y = pObject->FloatAttribute("y");
                float w = pObject->FloatAttribute("width");
                float h = pObject->FloatAttribute("height");

                const char *pValue = pObject->Attribute("name");
                std::stringstream ssv;
                ssv << pValue;
                Door *door = new Door(graphics, _world, ssv.str(), GPoint(x, y) * Globals::SPRITE_SCALE, GSize(w, h) * Globals::SPRITE_SCALE);
                _doors.push_back(door);

                pObject = pObject->NextSiblingElement("object");
            }
        }
        else if(ss.str() == "enemies"){

            XMLElement *eObject = objectgroupNode->FirstChildElement("object");
            const char *eName = eObject->Attribute("name");
            std::stringstream sse;
            sse << eName;

            while(eObject){
                if( sse.str() == "Snake"){
                    float x = eObject->FloatAttribute("x") * Globals::SPRITE_SCALE;
                    float y = eObject->FloatAttribute("y") * Globals::SPRITE_SCALE;
                    float w = eObject->FloatAttribute("width") * Globals::SPRITE_SCALE;

                    Snake *snake = new Snake(graphics, _world, GPoint(x, y));
                    snake->setBoundries((x + w), x);
                    _enemies.push_back(snake);
                }

                eObject = eObject->NextSiblingElement("object");
            }
        }
        /**
         * Other object group goes here
         * TODO : KEYS
         */


        objectgroupNode = objectgroupNode->NextSiblingElement("objectgroup");
    }
}

/**
 * Callback function called when a collision
 * appens betwenn two objects
 * @param o1
 * @param o2
 */
void Level::collisionReaction(GObject *o1, GObject *o2) {
    Player *player          = NULL;
    Enemy  *enemie          = NULL;
    Tile   *tile            = NULL;
    ActiveTile *activeTile1 = NULL;
    ActiveTile *activeTile2 = NULL;
    Door       *door        = NULL;
    Stairs     *stairs      = NULL;

    GObjectData d1 = o1->getGObjectData();
    GObjectData d2 = o2->getGObjectData();

    if(d1.player)
        player = d1.player;
    else if(d1.enemie)
        enemie = d1.enemie;
    else if(d1.activeTile)
        activeTile1 = d1.activeTile;
    else if(d1.tile)
        tile = d1.tile;
    else if(d1.door)
        door = d1.door;
    else if(d1.stairs)
        stairs = d1.stairs;

    if(d2.player)
        player = d2.player;
    else if(d2.enemie)
        enemie = d2.enemie;
    else if(d2.activeTile){
        if(activeTile1)
            activeTile2 = d2.activeTile;
        else
            activeTile1 = d2.activeTile;
    }
    else if(d2.tile)
        tile = d2.tile;
    else if(d2.door)
        door = d2.door;
    else if(d2.stairs)
        stairs = d2.stairs;

    if(player)
        player->setCollidingObject(player->getBody() == o1 ? o2 : o1);

    if(player && tile){
        Sides::Side side = player->getBody()->getCollisionSide(tile->getBBox());

        if(player->getBody()->getDelegate())
            player->getBody()->getDelegate()->touchTile(tile, side);
    }
    else if(player && stairs){
        _player->setOnStairs(true);
    }
    else if(player && door){
        if(_delegate)
            _delegate->playerReachDoor(door);
    }
    else if(player && enemie){
        _player->gainHealth(-1);
    }
    else if(enemie && tile){
        Sides::Side side = enemie->getBody()->getCollisionSide(tile->getBBox());

        if(enemie->getBody()->getDelegate())
            enemie->getBody()->getDelegate()->touchTile(tile, side);
    }
    else if(activeTile1 && tile){
        Sides::Side side = activeTile1->getBBox()->getCollisionSide(tile->getBBox());

        if(activeTile1->getBBox()->getDelegate())
            activeTile1->getBBox()->getDelegate()->touchTile(tile, side);
    }
    else if(activeTile1 && player){
        std::cout << "PLAYER ACTIVE TILE" << std::endl;

        Sides::Side side = player->getBody()->getCollisionSide(activeTile1->getBBox());

        if(player->getBody()->getDelegate())
            player->getBody()->getDelegate()->touchActiveTile(activeTile1, side);
    }
    else if(activeTile1 && enemie){
        Sides::Side side = enemie->getBody()->getCollisionSide(activeTile1->getBBox());

        if(enemie->getBody()->getDelegate())
            enemie->getBody()->getDelegate()->touchActiveTile(activeTile1, side);
    }
    else if(activeTile1 && activeTile2){
        std::cout << "ACTIVE TILE  ACTIVE TILE" << std::endl;

        ActiveTile *t1 = (activeTile1->getBBox()->getVelocity().x != 0 ? activeTile1 : activeTile2);
        ActiveTile *t2 = (t1 == activeTile1 ? activeTile2 : activeTile1);

        Sides::Side side = t1->getBBox()->getCollisionSide(t2->getBBox());

        if(t1->getBBox()->getDelegate())
            t1->getBBox()->getDelegate()->touchActiveTile(t2, side);
    }
}

void Level::collisionEnded(GObject *o1, GObject *o2) {
    /*Player *player          = NULL;
    Enemy  *enemie          = NULL;
    Tile   *tile            = NULL;
    ActiveTile *activeTile1 = NULL;
    ActiveTile *activeTile2 = NULL;
    Door       *door        = NULL;
    Stairs     *stairs      = NULL;

    GObjectData d1 = o1->getGObjectData();
    GObjectData d2 = o2->getGObjectData();

    if(d1.player)
        player = d1.player;
    else if(d1.enemie)
        enemie = d1.enemie;
    else if(d1.activeTile)
        activeTile1 = d1.activeTile;
    else if(d1.tile)
        tile = d1.tile;
    else if(d1.door)
        door = d1.door;
    else if(d1.stairs)
        stairs = d1.stairs;

    if(d2.player)
        player = d2.player;
    else if(d2.enemie)
        enemie = d2.enemie;
    else if(d2.activeTile){
        if(activeTile1)
            activeTile2 = d2.activeTile;
        else
            activeTile1 = d2.activeTile;
    }
    else if(d2.tile)
        tile = d2.tile;
    else if(d2.door)
        door = d2.door;
    else if(d2.stairs)
        stairs = d2.stairs;

    if(player && activeTile1)
        activeTile1->getBBox()->setVelocityX(0);
    if(activeTile1 && tile){
        activeTile1->getBBox()->setBlockedLeft(false);
        activeTile1->getBBox()->setBlockedRight(false);
    }
    else if(activeTile1 && activeTile2){
        activeTile1->getBBox()->setBlockedLeft(false);
        activeTile1->getBBox()->setBlockedRight(false);
        activeTile2->getBBox()->setBlockedLeft(false);
        activeTile2->getBBox()->setBlockedRight(false);
    }*/
}

void Level::intersectDetected(GObject *o1, GObject *o2, GVector MTD) {
    Player *player          = NULL;
    Enemy  *enemie          = NULL;
    Tile   *tile            = NULL;
    ActiveTile *activeTile1 = NULL;
    ActiveTile *activeTile2 = NULL;
    Door       *door        = NULL;
    Stairs     *stairs      = NULL;

    GObjectData d1 = o1->getGObjectData();
    GObjectData d2 = o2->getGObjectData();

    if(d1.player)
        player = d1.player;
    else if(d1.enemie)
        enemie = d1.enemie;
    else if(d1.activeTile)
        activeTile1 = d1.activeTile;
    else if(d1.tile)
        tile = d1.tile;
    else if(d1.door)
        door = d1.door;
    else if(d1.stairs)
        stairs = d1.stairs;

    if(d2.player)
        player = d2.player;
    else if(d2.enemie)
        enemie = d2.enemie;
    else if(d2.activeTile){
        if(activeTile1)
            activeTile2 = d2.activeTile;
        else
            activeTile1 = d2.activeTile;
    }
    else if(d2.tile)
        tile = d2.tile;
    else if(d2.door)
        door = d2.door;
    else if(d2.stairs)
        stairs = d2.stairs;

    if((player && tile)
       || (player && activeTile1)
       || (activeTile1 && activeTile2)
       || (activeTile1 && tile)
       || (enemie && tile)){
        float mult = 0.0f;
        if(o1->getObjectMode() == GPhy::DYNAMIC){
            mult = 0.5f;

            if(MTD.y < 0)
                o1->setGrounded(true);
            else
                o1->setGrounded(false);

            o1->setPosition(o1->getPosition() + MTD * mult);
            if(enemie && o1 == enemie->getBody() && MTD.x < -0.2)
                enemie->setDirection(Sides::LEFT);
            else if(enemie && o1 == enemie->getBody() && MTD.x > 0.2)
                enemie->setDirection(Sides::RIGHT);
        }

        if(o2->getObjectMode() == GPhy::DYNAMIC){
            o2->setPosition(o2->getPosition() - MTD * (1.0f - mult));
            o2->setLastMTD(GVector(-MTD.x, -MTD.y) * 10);

            if(MTD.y > 0)
                o2->setGrounded(true);
            else
                o2->setGrounded(false);

            if(enemie && o2 == enemie->getBody() && MTD.x > 0.2)
                enemie->setDirection(Sides::LEFT);
            else if(enemie && o2 == enemie->getBody() && MTD.x < -0.2)
                enemie->setDirection(Sides::RIGHT);
        }
    }
    else if(player && door){
        if(_delegate)
            _delegate->playerReachDoor(door);
    }
    else if(player && enemie)
        _player->gainHealth(-1);
    else if(player && stairs)
        _player->setOnStairs(true);
}

void Level::playerIsDead(Player *player) {
    player->revive(GPoint(_spawnPoint.x, _spawnPoint.y));
    if(_delegate)
        _delegate->resetLevel(this);
}