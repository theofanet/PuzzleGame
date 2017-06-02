//
// Created by Theo Fanet on 30/10/2016.
//

#include <GPhy/GCollision.h>
#include <GPhy/GRect.h>


bool GCollision::checkCollisions(GObject *o1, GObject *o2) {
    GPhy::Type t1 = o1->getObjectType();

    bool result = false;

    switch(t1){
        case GPhy::RECTANGLE:
            result = GRectCollision(static_cast<GRect*>(o1), o2);
            break;
        default:
            break;
    }

    if(result){
        o1->setColliding(true);
        o2->setColliding(true);
        intersect(o1, o2);
    }

    return result;
}


/*
 * GRects collisions test
 */
bool GCollision::GRectCollision(GRect* rect, GObject *o){
    GPhy::Type t = o->getObjectType();

    bool result = false;

    switch(t){
        case GPhy::RECTANGLE:
            result = GRect_GRect(rect, static_cast<GRect*>(o));
            break;
        default:
            break;
    }

    return result;
}

bool GCollision::GRect_GRect(GRect *r1, GRect *r2) {
    if((r2->getPosition().x >= r1->getPosition().x + r1->getSize().w)
       || (r2->getPosition().x + r2->getSize().w <= r1->getPosition().x)
       || (r2->getPosition().y >= r1->getPosition().y + r1->getSize().h)
       || (r2->getPosition().y + r2->getSize().h <= r1->getPosition().y))
        return false;
    else
        return true;
}

void GCollision::calculateInterval(GVector axis, GObject *obj, float &min, float &max) {
    std::vector<GPoint> pts = obj->getVertices();

    if(pts.size()){
        float d = axis.dotproduct(pts.at(0).asVector());
        min = max = d;
        for(std::vector<GPoint>::iterator it = pts.begin(); it != pts.end(); ++it){
            d = (*it).dotproduct(axis);
            if(d < min)
                min = d;
            else if(d > max)
                max = d;
        }
    }
}

bool GCollision::axisSeparatePolygons(GVector &axis, GObject *o1, GObject *o2) {
    float mina, maxa;
    float minb, maxb;

    calculateInterval(axis, o1, mina, maxa);
    calculateInterval(axis, o2, minb, maxb);

    if(mina > maxb || minb > maxa)
        return true;

    float d0 = maxa - minb;
    float d1 = maxb - mina;
    float depth = (d0 < d1) ? d0 : d1;

    float axisLengthSquared = axis.dotproduct(axis);

    axis *= depth / axisLengthSquared;

    return false;
}

GVector GCollision::findMTD(std::vector<GVector> pushVectors) {
    GVector MTD = pushVectors.at(0);
    float mind2 = pushVectors.at(0).dotproduct(pushVectors.at(0));

    for(std::vector<GVector>::iterator it = pushVectors.begin() + 1; it != pushVectors.end(); ++it){
        float d2 = (*it).dotproduct((*it));
        if(d2 < mind2){
            mind2 = d2;
            MTD = (*it);
        }
    }

    return MTD;
}

bool GCollision::intersect(GObject *o1, GObject *o2) {
    std::vector<GVector> axis;

    std::vector<GPoint> aPts = o1->getVertices();
    std::vector<GPoint> bPts = o2->getVertices();

    for(size_t j = aPts.size() - 1, i = 0; i < aPts.size(); j = i, i++){
        GPoint  e = aPts.at(i) - aPts.at(j);
        GVector n = GVector(-e.y, e.x);
        if(axisSeparatePolygons(n, o1, o2))
            return false;
        axis.push_back(n);
    }

    for(size_t j = bPts.size() - 1, i = 0; i < bPts.size(); j = i, i++){
        GPoint  e = bPts.at(i) - bPts.at(j);
        GVector n = GVector(-e.y, e.x);
        if(axisSeparatePolygons(n, o1, o2))
            return false;
        axis.push_back(n);
    }

    if(_delegate){
        GVector MTD = findMTD(axis);
        GVector d   = (o1->getPosition() - o2->getPosition()).asVector();
        if(d.dotproduct(MTD) < 0.0f)
            MTD = GVector(-MTD.x, -MTD.y);

        _delegate->intersectDetected(o1, o2, MTD);
    }

    return true;
}