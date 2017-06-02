//
// Created by Theo Fanet on 29/10/2016.
//

#ifndef GPHY_GWORLD_H
#define GPHY_GWORLD_H

#include <GPhy/GObject.h>
#include <GPhy/GRect.h>
#include <GPhy/GCollision.h>
#include <Globals.h>
#include <vector>
#include <list>
#include <algorithm>

class Graphics;


class GWorld {
public:
    GWorld(float gravity);
    ~GWorld();

    void update(float elapsedTime);
    void draw(Graphics &graphics);

    void addObject(GObject *o);
    inline void setPlayer(GObject *p){
        _player = p;
        p->addForce("gravity", 0, _gravity);
    }

    inline void setCollisionDelegate(GCollisionDelegate *delegate){
        _collision.setDelegate(delegate);
        _collisionDelegate = delegate;
    }

private:
    float _gravity;

    GObject *_player;

    GCollision _collision;
    GCollisionDelegate *_collisionDelegate;
    LinkedList _objects;

    std::vector<std::pair<GObject*, GObject*>> _colliding[2];
    int _currentCollidingArray;
};


#endif //GPHY_GWORLD_H
