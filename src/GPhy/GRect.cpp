//
// Created by Theo Fanet on 29/10/2016.
//

#include <GPhy/GRect.h>
#include <GGraph/Graphics.h>
#include <cmath>

GRect::GRect(GPoint pos, GSize size) : GObject(){
    _size     = size;
    _position = pos;

    _boundingBox.pos  = pos;
    _boundingBox.size = size;

    _type = GPhy::RECTANGLE;
}

GRect::~GRect() {

}

void GRect::update(float elapseTime) {
    GObject::update(elapseTime);
}

void GRect::draw(Graphics &graphics) {

    if(_blockedRight)
        SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 255, 255);
    else if(_blockedLeft)
        SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 255, 255, 255);
    else if(isColliding())
        SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 0, 0, 255);
    else
        SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 255, 0, 255);

    float x = _position.x - graphics.getCamera().getPosition().x;
    float y = _position.y - graphics.getCamera().getPosition().y;

    SDL_Rect bRect = { (int)x, (int)y, (int)_size.w, (int)_size.h };
    SDL_RenderDrawRect(graphics.getRenderer(), &bRect);

    if(_blockedRight)
        SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 255, 100);
    else if(_blockedLeft)
        SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 255, 255, 100);
    else if(isColliding())
        SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 0, 0, 100);
    else
        SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 255, 0, 100);
    SDL_SetRenderDrawBlendMode(graphics.getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(graphics.getRenderer(), &bRect);
}

Sides::Side GRect::getCollisionSide(GRect *rect) {
    float w = 0.5f * (_size.w + rect->getSize().w);
    float h = 0.5f * (_size.h + rect->getSize().h);
    float dx = getCenterX() - rect->getCenterX();
    float dy = getCenterY() - rect->getCenterY();

    float wy = w * dy;
    float hx = h * dx;

    if(std::fabs(dx) > w || std::fabs(dy) > h)
        return Sides::NONE;

    if (wy > hx){
        if (wy > -hx)
            return Sides::TOP;
        else
            return Sides::RIGHT;
    }
    else{
        if (wy > -hx)
            return Sides::LEFT;
        else
            return Sides::BOTTOM;
    }
}


std::vector<GPoint> GRect::getVertices() {
    std::vector<GPoint> pts;

    pts.push_back(_position);
    pts.push_back(_position + GPoint(_size.w, 0));
    pts.push_back(_position + GPoint(_size.w, _size.h));
    pts.push_back(_position + GPoint(0, _size.h));

    return pts;
}