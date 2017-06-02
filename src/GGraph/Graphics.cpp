//
// Created by Theo Fanet on 22/10/2016.
//

#include <GGraph/Graphics.h>
#include <Globals.h>
#ifdef __WIN32__
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif


Graphics::Graphics() {
    SDL_CreateWindowAndRenderer(Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 0, &_window, &_renderer);
    SDL_SetWindowTitle(_window, "Game Test");
}

Graphics::~Graphics() {
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);

    for(std::map<std::string, SDL_Surface*>::iterator it = _spriteSheets.begin(); it != _spriteSheets.end(); ++it)
        SDL_FreeSurface((*it).second);
}

SDL_Surface* Graphics::loadImage(const std::string &filePath) {
    if(_spriteSheets.count(filePath) == 0)
        _spriteSheets[filePath] = IMG_Load(filePath.c_str());

    if(_spriteSheets[filePath] == NULL)
        std::cout << "Unable to load image" << SDL_GetError() << std::endl;

    return _spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture *src, SDL_Rect *srcRect, SDL_Rect *dstRect, float angle, SDL_Point *center, SDL_RendererFlip flip) {
    dstRect->x -= _camera.getPosition().x;
    dstRect->y -= _camera.getPosition().y;

    SDL_RenderCopyEx(_renderer, src, srcRect, dstRect, angle, center, flip);
}

void Graphics::flip() {
    SDL_RenderPresent(_renderer);
}

void Graphics::clear() {
    SDL_RenderClear(_renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
    return _renderer;
}

SDL_Window* Graphics::getWindow() const {
    return _window;
}

void Graphics::setWindowTilte(std::string title) {
    SDL_SetWindowTitle(_window, title.c_str());
}