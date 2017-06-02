//
// Created by glab on 11/6/16.
//

#ifndef PUZZELGAME_FONT_H
#define PUZZELGAME_FONT_H

#ifdef __WIN32__
#include <SDL2/SDL_ttf.h>
#else
#include <SDL2_ttf/SDL_ttf.h>
#endif

#include <string>

class Graphics;

class Font {
public:
    Font();
    Font(std::string font, int size);
    ~Font();

    inline void setColor(int r, int g, int b){
        _r = r;
        _g = g;
        _b = b;
    }

    inline void setColorR(int r){_r = r;}
    inline void setColorG(int g){_g = g;}
    inline void setColorB(int b){_b = b;}

    void draw(Graphics &graphics, std::string text, int posX, int posY);

private:
    int _r, _g, _b;
    TTF_Font *_font;
    SDL_Rect _destRect;

    void setFont(std::string font, int size);
};


#endif //PUZZELGAME_FONT_H
