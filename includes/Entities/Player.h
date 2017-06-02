//
// Created by Theo Fanet on 22/10/2016.
//

#ifndef TESTPROG_PLAYER_H
#define TESTPROG_PLAYER_H

#include <Globals.h>
#include <GGraph/AnimatedSprite.h>
#include <list>
#include <GPhy/GRect.h>
#include <GGraph/Graphics.h>
#include <GPhy/GWorld.h>
#include "LinkedList.h"


class GWorld;
class GRect;
class Level;

class Graphics;

class Rect : public LinkedListElement{
public:
    Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h){}

    SDL_Rect toSDLRect(){
        SDL_Rect r = { x, y, w, h };
        return r;
    }

    int x;
    int y;
    int w;
    int h;
};

class Player;

class PlayerDelegate{
public:
    virtual void playerIsDead(Player *player){ }
};



class Player : public AnimatedSprite, public AnimatedSpriteDelegate, public GObjectDelegate{
public:
    Player();
    Player(Graphics &graphics, Vector2 spawnPoint);
    ~Player();

    void draw(Graphics &graphics);
    void update(float elapsedTime);

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void stopMoving();
    void jump();

    void setupAnimations();

    void animationDone(AnimatedSprite *apsite, std::string currentAnimation);

    const inline int getMaxHalth() const { return _maxHealth; }
    const inline int getCurrentHealth() const { return _currentHealth; }
    void gainHealth(int amount);
    void revive(GPoint pos);

    const inline Direction getDirection() const { return _facing; }

    const inline bool isGrounded() const { return _grounded; }
    inline void setGrounded(bool g = true){ _grounded = g; }

    inline void setRunning(bool running = true){ _running = running; }
    inline bool isRunning(){ return _running; }

    inline GRect* getBody(){ return _body; }
    inline GObject* getCollingObject(){ return _collidingObject; }
    inline void setCollidingObject(GObject *obj){ _collidingObject = obj; }

    inline void setWorld(GWorld *world){ _world = world; }

    void touchActiveTile(ActiveTile *tile, Sides::Side side);
    void touchTile(Tile *tile, Sides::Side side);

    inline bool isOnStairs(){ return _onStairs; }
    void setOnStairs(bool o);

    inline bool isHurt(){ return _hurt; }
    inline void setHurt(bool h = true){ _hurt = h; }

    inline void setPlayerDelegate(PlayerDelegate *delegate){ _playerDelegate = delegate; }

private:
    GWorld *_world;
    GRect  *_body;

    bool _hurt;
    int _iHurt;
    float _iBeingHurt;

    GObject *_collidingObject;

    PlayerDelegate *_playerDelegate;

    Direction _facing;
    bool _grounded;
    bool _running;
    bool _onStairs;

    int _maxHealth;
    int _currentHealth;
};

#endif //TESTPROG_PLAYER_H
