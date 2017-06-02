//
// Created by Theo Fanet on 22/10/2016.
//

#ifndef TESTPROG_INPUT_H
#define TESTPROG_INPUT_H

#include <map>
#include <SDL2/SDL.h>

class Input{
public:
    void beginNewFrame();
    void keyUpEvent(const SDL_Event& event);
    void keyDownEvent(const SDL_Event& event);

    bool wasKeyPressed(SDL_Scancode key);
    bool wasKeyReleased(SDL_Scancode key);
    bool isKeyHeld(SDL_Scancode key);

private:
    std::map<SDL_Scancode, bool> _heldKeys;
    std::map<SDL_Scancode, bool> _pressedKeys;
    std::map<SDL_Scancode, bool> _releasedKey;
};

#endif //TESTPROG_INPUT_H
