//
// Created by Theo Fanet on 29/10/2016.
//

#include <GPhy/GWorld.h>


GWorld::GWorld(float gravity) {
    _gravity           = gravity * 5;
    _collisionDelegate = NULL;
    _currentCollidingArray = 0;

}

GWorld::~GWorld() {
    GObject *o = static_cast<GObject*>(_objects.getFirst());
    while(o){
        GObject *old = o;
        o = static_cast<GObject*>(o->next);
        delete_obj(old);
    }
}

/**
 * Add an object to the world's object list
 */
void GWorld::addObject(GObject *o) {
    o->addForce("gravity", 0, _gravity);
    _objects.push(o);
}


/**
 * Updating the world,
 * Updates every objects
 */
void GWorld::update(float elapsedTime) {
    GObject *o, *o2;

    _colliding[_currentCollidingArray].clear();

    if(_player) {
        _player->update(elapsedTime);

        o = static_cast<GObject*>(_objects.getFirst());
        while(o){
            /*if(_collision.checkCollisions(_player, o)){
                o->setColliding(true);
                _player->setColliding(true);

                _colliding[_currentCollidingArray].push_back(std::make_pair(_player, o));

                if(_collisionDelegate)
                    _collisionDelegate->collisionReaction(_player, o);
            }*/

            _collision.checkCollisions(o, _player);
            o = static_cast<GObject*>(o->next);
        }
    }

    o = static_cast<GObject*>(_objects.getFirst());
    while(o){
        o->update(elapsedTime);
        o = static_cast<GObject*>(o->next);
    }

    o = static_cast<GObject*>(_objects.getFirst());
    while(o){
        o2 = static_cast<GObject*>(_objects.getFirst());
        while(o2){
            if(o != o2){
                /*if(_collision.checkCollisions(o, o2)){
                    o->setColliding(true);
                    o2->setColliding(true);

                    _colliding[_currentCollidingArray].push_back(std::make_pair(o, o2));

                    if(_collisionDelegate)
                        _collisionDelegate->collisionReaction(o, o2);
                }*/

                _collision.checkCollisions(o, o2);
            }

            o2 = static_cast<GObject*>(o2->next);
        }
        o = static_cast<GObject*>(o->next);
    }

    int other = (_currentCollidingArray == 0 ? 1 : 0);
    for(std::vector<std::pair<GObject*, GObject*>>::iterator it = _colliding[other].begin(); it != _colliding[other].end(); ++it){
        if(std::find(_colliding[_currentCollidingArray].begin(), _colliding[_currentCollidingArray].end(), (*it)) != _colliding[_currentCollidingArray].end() && _collisionDelegate)
            _collisionDelegate->collisionEnded((*it).first, (*it).second);
    }
    _currentCollidingArray = other;
}


/**
 * Drawing the world,
 * Draws every objects
 */
void GWorld::draw(Graphics &graphics) {
    GObject *o = static_cast<GObject*>(_objects.getFirst());
    while(o){
        o->draw(graphics);
        o = static_cast<GObject*>(o->next);
    }

    if(_player)
        _player->draw(graphics);
}