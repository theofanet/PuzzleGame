//
// Created by Theo Fanet on 29/10/2016.
//

#include <GPhy/GObject.h>


GObject::GObject() {
    _position = GPoint();
    _velocity = GVector();

    _type = GPhy::NONE;
    _mode = GPhy::DYNAMIC;

    _colliding = false;
    _grounded  = false;
    _moving    = false;

    _gravityScale = 1.0f;

    _blockedLeft  = false;
    _blockedRight = false;

    _delegate = NULL;
}

GObject::~GObject() {

}

void GObject::addForce(std::string name, float x, float y, float r, float g, float b) {
    GForce f;

    if(_forces.count(name) != 0)
        f = _forces[name];

    f.name = name;

    f.x = x;
    f.y = y;

    f.r = r;
    f.g = g;
    f.b = b;

    _forces[name] = f;
}

GForce GObject::getForce(std::string name) {
    if(_forces.find(name) != _forces.end())
        return _forces[name];

    return GForce();
}

void GObject::update(float elapsedTime) {
    _moving = false;

    if(_mode == GPhy::DYNAMIC){
        GVector acceleration;
        for(GForceList::iterator it = _forces.begin(); it != _forces.end(); ++it) {
            if((*it).first != "gravity" || !_grounded || _velocity != GVector())
                acceleration += (*it).second.getVector() * ((*it).first == "gravity" ? _gravityScale : 1.0f);
        }

        GPoint oldPos = _position;
        _velocity += acceleration * (elapsedTime / 1000);
        _position += _velocity;

        if(_position != oldPos) {
            _grounded = false;
            _moving   = true;
        }

        _grounded = false;
    }
}
