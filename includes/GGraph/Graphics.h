//
// Created by Theo Fanet on 22/10/2016.
//

#ifndef TESTPROG_GRAPHICS_H
#define TESTPROG_GRAPHICS_H

#include <SDL2/SDL.h>

#include <string>
#include <map>

#include "Camera.h"

class Graphics{
public:
    Graphics();
    ~Graphics();

    /*
     * Load image in _spriteSheet if it doesn't already exists
     * Returns the image from _spriteSheet
     */
    SDL_Surface* loadImage(const std::string &filePath);

    /**
     * Draw a texture to a certain part of the screen
     */
    void blitSurface(SDL_Texture* src, SDL_Rect* srcRect, SDL_Rect* dstRect, float angle = 0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    /**
     * Renders everything to the screen
     */
    void flip();

    /**
     * Clears the screen
     */
    void clear();

    void setWindowTilte(std::string title);

    /**
     * Returns the renderer
     */
    SDL_Renderer* getRenderer() const;
    SDL_Window* getWindow() const;

    inline Camera getCamera(){ return _camera; }
    inline void setCameraPosition(GPoint pos){ _camera.setPosition(pos); }
    inline void setCameraX(float x){ _camera.setPositionX(x); }
    inline void setCameraY(float y){ _camera.setPositionY(y); }

private:
    SDL_Window*   _window;
    SDL_Renderer* _renderer;

    Camera _camera;

    std::map<std::string, SDL_Surface*> _spriteSheets;
};

#endif //TESTPROG_GRAPHICS_H
