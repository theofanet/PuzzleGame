//
// Created by Theo Fanet on 22/10/2016.
//


#include <GGraph/AnimatedSprite.h>
#include <GGraph/Graphics.h>


AnimatedSprite::AnimatedSprite() {

}

AnimatedSprite::AnimatedSprite(Graphics &graphics, const std::string &filePath, int srcX, int srcY, int w, int h,
                               float posX, float posY, float timeToUpdate) :
        Sprite(graphics, filePath, srcX, srcY, w, h, posX, posY){
    _frameIndex           = 0;
    _visible              = true;
    _timeToUpdate         = timeToUpdate;
    _currentAnimationOnce = false;
    _currentAnimation     = "";
    _timeElapsed          = 0;
    _delegate             = NULL;
}

AnimatedSprite::~AnimatedSprite() {

}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int w, int h, Vector2 offset) {
    std::vector<SDL_Rect> rectangles;

    for(int i = 0; i < frames; i++){
        SDL_Rect newRect = { (x + i) * w, (y * h), w, h };
        rectangles.push_back(newRect);
    }

    _animations[name] = rectangles;
    _offsets[name]    = offset;
}

void AnimatedSprite::clearAnimations() {
    _animations.clear();
    _offsets.clear();
}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
    _currentAnimationOnce = once;
    setVisible(true);

    if(_currentAnimation != animation){
        _currentAnimation = animation;
        _frameIndex       = 0;
    }
}

void AnimatedSprite::setVisible(bool visible) {
    _visible = visible;
}

void AnimatedSprite::stopAnimation() {
    _frameIndex = 0;
    if(_delegate)
        _delegate->animationDone(this, _currentAnimation);
}

void AnimatedSprite::update(float elapsedTime) {
    if(_visible){
        Sprite::update();

        _timeElapsed += elapsedTime;
        if(_timeElapsed > _timeToUpdate){
            _timeElapsed -= _timeToUpdate;
            if(_frameIndex < _animations[_currentAnimation].size() - 1)
                _frameIndex++;
            else{
                if(_currentAnimationOnce)
                    setVisible(false);
                stopAnimation();
            }
        }
    }
}

void AnimatedSprite::draw(Graphics &graphics, int x, int y, float angle, SDL_Point *center, SDL_RendererFlip flip) {
    if(_visible && _animations.count(_currentAnimation) != 0){
        SDL_Rect dstRect;

        dstRect.x = x + _offsets[_currentAnimation].x;
        dstRect.y = y + _offsets[_currentAnimation].y;
        dstRect.w = _srcRect.w * Globals::SPRITE_SCALE;
        dstRect.h = _srcRect.h * Globals::SPRITE_SCALE;

        SDL_Rect srcRect = _animations[_currentAnimation][_frameIndex];
        graphics.blitSurface(_spriteSheet, &srcRect, &dstRect, angle, center, flip);
    }
}