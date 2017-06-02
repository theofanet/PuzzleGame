//
// Created by Theo Fanet on 29/10/2016.
//

#ifndef GPHY_GPHY_H
#define GPHY_GPHY_H

#include <iostream>
#include <math.h>
#include <map>

namespace GPhy{
    enum Mode{
        DYNAMIC,
        STATIC
    };

    enum Type{
        NONE,
        RECTANGLE
    };
}

struct GVector{
    float x, y;

    GVector(){
        x = 0;
        y = 0;
    }

    GVector(float X, float Y){
        x = X;
        y = Y;
    }

    float normalise(){
        float mag = magnitude();
        x = (mag > 0 ? x / mag : x);
        y = (mag > 0 ? y / mag : y);
        return mag;
    }

    bool isNull(){
        return (x == 0 && y == 0);
    }

    GVector copy(){
        return GVector(x, y);
    }

    float dotproduct(GVector v){
        return (x * v.x) + (y * v.y);
    }

    float magnitude(){
        return (float)sqrt(x * x + y * y);
    }

    GVector project(GVector v){
        GVector N = copy();
        float dot = dotproduct(v);
        N.normalise();
        GVector result;
        result.x = (dot / (N.x * N.x)) * x;
        result.y = (dot / (N.y * N.y)) * y;
        return result;
    }

    void rotate(float angle){
        float xt = (x * cosf(angle)) - (y * sinf(angle));
        float yt = (y * cosf(angle)) + (x * sinf(angle));
        x = xt;
        y = yt;
    }

    GVector operator*(float scalar) const{
        return GVector(x * scalar, y * scalar);
    }

    GVector operator*(GVector v) const{
        return GVector(x * v.x, y * v.y);
    }

    GVector operator/(GVector v) const{
        return GVector(x / v.x, y / v.y);
    }

    GVector operator+(GVector v) const{
        return GVector(x + v.x, y + v.y);
    }

    const GVector &operator+=(const GVector &v){
        x += v.x;
        y += v.y;
        return *this;
    }

    const GVector &operator*=(const float v){
        x *= v;
        y *= v;

        return *this;
    }

    bool operator==(GVector v) const{
        return (x == v.x && y == v.y);
    }

    bool operator!=(GVector v) const{
        return (x != v.x || y != v.y);
    }
};

struct GPoint{
    float x, y;

    GPoint(float X = 0, float Y = 0){
        x = X;
        y = Y;
    }

    GPoint operator-(float e){
        return GPoint(x - e, y - e);
    }

    GPoint operator-(GPoint P){
        return GPoint(x - P.x, y - P.y);
    }

    GPoint operator-(GVector P){
        return GPoint(x - P.x, y - P.y);
    }

    GPoint operator+(GPoint P){
        return GPoint(x + P.x, y + P.y);
    }

    GPoint operator+(GVector P){
        return GPoint(x + P.x, y + P.y);
    }

    GVector asVector(){
        return GVector(x, y);
    }

    float dotproduct(GVector v){
        return (x * v.x) + (y * v.y);
    }

    float dotproduct(GPoint p){
        return (x * p.x) + (y * p.y);
    }

    const GPoint &operator+=(const GVector &v){
        x += v.x;
        y += v.y;

        return *this;
    }


    const GPoint &operator-=(const GVector &v){
        x -= v.x;
        y -= v.y;

        return *this;
    }

    GPoint operator*(int i){
        return GPoint(x * i, y * i);
    }

    bool operator==(GPoint v) const{
        return (x == v.x && y == v.y);
    }

    bool operator!=(GPoint v) const{
        return (x != v.x || y != v.y);
    }
};

struct GSize{
    GSize() : w(1), h(1){}
    GSize(float w, float h) : w(w), h(h){}

    GSize operator*(int i){
        return GSize(w * i, h * i);
    }

    float w, h;
};

struct GForce{
    GForce(){
        x = 0;
        y = 0;

        name = "f";

        r = 0;
        g = 0;
        b = 1;
    }

    float x, y;
    float r, g, b;

    std::string  name;

    GVector getVector(){
        return GVector(x, y);
    }
};

struct GBBox{
    GPoint pos;
    GSize  size;
};

typedef std::map<std::string, GForce> GForceList;

#endif //GPHY_GPHY_H
