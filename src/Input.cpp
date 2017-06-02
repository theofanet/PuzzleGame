//
// Created by Theo Fanet on 22/10/2016.
//


#include <Input.h>

void Input::beginNewFrame() {
    _pressedKeys.clear();
    _releasedKey.clear();
}

void Input::keyUpEvent(const SDL_Event &event) {
    _releasedKey[event.key.keysym.scancode] = true;
    _heldKeys[event.key.keysym.scancode] = false;
}

void Input::keyDownEvent(const SDL_Event &event) {
    _pressedKeys[event.key.keysym.scancode] = true;
    _heldKeys[event.key.keysym.scancode] = true;
}

bool Input::wasKeyPressed(SDL_Scancode key) {
    return _pressedKeys[key];
}

bool Input::wasKeyReleased(SDL_Scancode key) {
    return _releasedKey[key];
}

//Check if a certain key is currently held
bool Input::isKeyHeld(SDL_Scancode key) {
    return _heldKeys[key];
}
