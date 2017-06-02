//
// Created by Theo Fanet on 30/10/2016.
//

#ifndef GPHY_GCOLLISION_H
#define GPHY_GCOLLISION_H

#include <vector>
#include "GPhy/GPhy.h"

class GObject;
class GRect;

class GCollisionDelegate{
public:
    virtual void collisionReaction(GObject *o1, GObject *o2){}
    virtual void collisionEnded(GObject *o1, GObject *o2){}
    virtual void intersectDetected(GObject *o1, GObject *o2, GVector MTD){}
};


class GCollision {
public:
    GCollision(){
        _delegate = NULL;
    }

    bool checkCollisions(GObject *o1, GObject *o2);

    /**
     * Separated Axis Method
     */
    bool intersect(GObject *o1, GObject *o2);

    void setDelegate(GCollisionDelegate *delegate){ _delegate = delegate; }

private:
    GCollisionDelegate *_delegate;

    /**
     * GRect collisions
     */
    bool GRectCollision(GRect* rect, GObject *o);
    bool GRect_GRect(GRect *r1, GRect *r2);

    /**
     * Separated Axis Method
     */
    void calculateInterval(GVector axis, GObject *obj, float &min, float &max);
    bool axisSeparatePolygons(GVector &axis, GObject *o1, GObject *o2);
    GVector findMTD(std::vector<GVector> pushVectors);
};


#endif //GPHY_GCOLLISION_H
