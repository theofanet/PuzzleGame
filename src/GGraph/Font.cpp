//
// Created by glab on 11/6/16.
//

#include "GGraph/Font.h"
#include <GGraph/Graphics.h>

Font::Font() {}

Font::Font(std::string font, int size) {
    this->setFont(font, size);
}

Font::~Font() {
    TTF_CloseFont(_font);
}

void Font::setFont(std::string font, int size) {
    this->_font = TTF_OpenFont(font.c_str(), size);
}

void Font::draw(Graphics &graphics, std::string text, int posX, int posY) {
    SDL_Color color          = {(Uint8)this->_r, (Uint8)this->_g, (Uint8)this->_b};
    SDL_Surface *textSurface = TTF_RenderText_Blended(this->_font, text.c_str(), color);
    SDL_Texture *texture     = SDL_CreateTextureFromSurface(graphics.getRenderer(), textSurface);

    this->_destRect = {posX, posY, textSurface->w, textSurface->h};

    SDL_FreeSurface(textSurface);

    graphics.blitSurface(texture, NULL, &this->_destRect);
}