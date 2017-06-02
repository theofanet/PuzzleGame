//
// Created by Theo Fanet on 29/10/2016.
//

#ifndef GPHY_GOBJECT_H
#define GPHY_GOBJECT_H

#include <map>
#include <LinkedList.h>
#include <GPhy/GPhy.h>
#include <vector>
#include "Globals.h"


class Graphics;
class GObject;
class Player;
class Tile;
class ActiveTile;
class Enemy;
class Door;
class Stairs;

class GObjectDelegate{
public:
    virtual void touchTile(Tile *player, Sides::Side side){}
    virtual void touchActiveTile(ActiveTile *tile, Sides::Side side){}
};

class GObjectData{
public:
    Player     *player = NULL;
    Tile       *tile   = NULL;
    ActiveTile *activeTile = NULL;
    Enemy      *enemie = NULL;
    Door       *door   = NULL;
    Stairs     *stairs = NULL;
};

class GObject : public LinkedListElement {
public:
    GObject();
    virtual ~GObject();

    virtual void update(float elapsedTime);
    virtual void draw(Graphics &graphics){}

    inline GPoint getPosition(){ return _position; }
    inline void setPosition(GPoint pos){ _position = pos; }
    inline void setPosition(float x, float y){ setPosition(GPoint(x, y)); }
    inline void setPositionX(float x){ _position.x = x; }
    inline void setPositionY(float y){ _position.y = y; }

    inline GVector getVelocity(){ return _velocity; }
    inline void setVelocity(GVector pos){ _velocity = pos; }
    inline void setVelocity(float x, float y){ setVelocity(GVector(x, y)); }
    inline void setVelocityX(float x){ _velocity.x = x; }
    inline void setVelocityY(float y){ _velocity.y = y; }

    void addForce(std::string name, float x, float y, float r = 0.0, float g = 0.0, float b = 0.0);
    GForce getForce(std::string name);

    inline void applyImpulse(GVector impulse){ _velocity += impulse; }

    inline void setColliding(bool c){ _colliding = c; }
    inline bool isColliding(){ return _colliding; }

    inline GPhy::Type getObjectType(){ return _type; }
    inline GPhy::Mode getObjectMode(){ return _mode; }
    inline void setObjectMode(GPhy::Mode m){ _mode = m; }

    inline bool isGrounded(){ return _grounded; }
    inline void setGrounded(bool g){ _grounded = g; if(_grounded) _velocity = GVector(); }

    inline GBBox getBBox(){ return _boundingBox; }

    inline bool isMoving(){ return _moving; }

    inline void setBlockedLeft(bool b){ _blockedLeft = b; }
    inline void setBlockedRight(bool b){ _blockedRight = b; }
    inline bool isBlockedLeft(){ return _blockedLeft; }
    inline bool isBlockedRight(){ return _blockedRight; }

    inline void setDelegate(GObjectDelegate *delegate){ _delegate = delegate; }
    inline GObjectDelegate* getDelegate(){ return _delegate; }

    inline void setGObjectData(GObjectData data){ _data = data; }
    inline GObjectData getGObjectData(){ return _data; }

    inline void setGravityScale(float g){ _gravityScale = g; }
    inline float getGravityScale(){ return _gravityScale; }

    virtual std::vector<GPoint> getVertices() = 0;

    inline GVector getLastMTD(){ return _lastMTD; }
    inline void setLastMTD(GVector MTD){ _lastMTD = MTD; }

protected:
    GPoint  _position;
    GVector _velocity;

    GPhy::Type _type;
    GPhy::Mode _mode;

    bool _colliding;
    bool _grounded;
    bool _moving;
    bool _blockedLeft;
    bool _blockedRight;

    float _gravityScale;

    GObjectDelegate *_delegate;
    GObjectData _data;

    GBBox _boundingBox;

    GVector _lastMTD;

    GForceList _forces;
};


#endif //GPHY_GOBJECT_H
