//
// Created by Theo Fanet on 22/10/2016.
//

#ifndef TESTPROG_ANIMATEDSPRITE_H_H
#define TESTPROG_ANIMATEDSPRITE_H_H

#include <map>
#include <string>
#include <vector>

#include <Globals.h>
#include <GGraph/Sprite.h>


class Graphics;
class AnimatedSprite;

class AnimatedSpriteDelegate {
public:
    virtual void animationDone(AnimatedSprite *apsite, std::string currentAnimation){}
};


class AnimatedSprite : public Sprite{
public:
    AnimatedSprite();
    AnimatedSprite(Graphics &graphics, const std::string &filePath, int srcX, int srcY, int w, int h, float posX, float posY, float timeToUpdate);
    ~AnimatedSprite();

    /**
     * Plays the requested animation if it's not already playing
     */
    void playAnimation(std::string animation, bool once = false);

    /**
     * updates the animation frame with timer
     */
    virtual void update(float elapsedTime);

    /**
     * Draw the sprite's current frame to the screen
     */
    void draw(Graphics &graphics, int x, int y, float angle = 0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    /**
     * Change the visibility of the animated sprite
     */
    void setVisible(bool visible);

    /**
     * Adds an animation to the animations map
     */
    void addAnimation(int frames, int x, int y, std::string name, int w, int h, Vector2 offset);

    /**
     * Reset frameIndex for current animation
     */
    inline void resetAnimation(){ _frameIndex = 0; }

    /**
     * Set delegate
     */
    inline void setDelegate(AnimatedSpriteDelegate *delegate){ _delegate = delegate; }

protected:
    double  _timeToUpdate;
    bool    _currentAnimationOnce;
    std::string _currentAnimation;

    /**
     * Clears the animation maps
     */
    void clearAnimations();

    /**
     * Stops the current animation
     */
    void stopAnimation();

    /**
     * Blocks to create animations in childs classes
     */
    virtual void setupAnimations() = 0;

    std::map<std::string, std::vector<SDL_Rect>> _animations;
    std::map<std::string, Vector2> _offsets;

    AnimatedSpriteDelegate *_delegate;

    int    _frameIndex;
    double _timeElapsed;
    bool   _visible;
};

#endif //TESTPROG_ANIMATEDSPRITE_H_H
