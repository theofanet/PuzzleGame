//
// Created by Theo Fanet on 29/10/2016.
//

#ifndef GPHY_GRECT_H
#define GPHY_GRECT_H

#include <GPhy/GObject.h>
#include "Globals.h"

class GRect : public GObject{
public:
    GRect(GPoint pos, GSize size);
    ~GRect();

    void update(float elapseTime);
    void draw(Graphics &graphics);

    inline GSize getSize(){ return _size; }
    inline void setSize(GSize size){ _size = size; }
    inline void setSize(float w, float h){ setSize(GSize(w, h)); }
    inline void setSizeW(float w){ _size.w = w; }
    inline void setSizeH(float h){ _size.h = h; }

    const float getLeft() const { return _position.x; }
    const float getRight() const { return _position.x + _size.w; }
    const float getBottom() const { return _position.y + _size.h; }
    const float getTop() const { return _position.y ; }

    inline float getCenterX(){ return _position.x + (_size.w / 2); }
    inline float getCenterY(){ return _position.y + (_size.h / 2); }

    Sides::Side getCollisionSide(GRect *rect);

    std::vector<GPoint> getVertices();

private:
    GSize _size;
};


#endif //GPHY_GRECT_H
