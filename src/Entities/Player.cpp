//
// Created by Theo Fanet on 22/10/2016.
//
#include "Entities/Player.h"
#include "Level.h"
#include "GPhy/GWorld.h"
#include <cmath>

namespace PlayerConstants{
    const float WALK_SPEED    = .5f;
    const float RUN_MULT      = 2.0f;
    const float JUMP_SPEED    = -7.0f;
    const int   MAX_HEALTH    = 3;
    const float MAX_TIME_HURT = 5000.0f;
    const int   HURT_TIME_SWITCH = 20;
}


Player::Player() {
    delete_obj(_boundingBox);
}

Player::Player(Graphics &graphics, Vector2 spawnPoint) : AnimatedSprite(graphics, "content/sprites/characters.png", 0, 0, 32, 32, spawnPoint.x, spawnPoint.y, 200){
    setupAnimations();
    playAnimation("Idle");

    _facing = RIGHT;

    _delegate = this;
    _playerDelegate = NULL;

    _maxHealth     = PlayerConstants::MAX_HEALTH;
    _currentHealth = _maxHealth;

    _body = new GRect(GPoint(_x, _y), GSize(10, 17) * Globals::SPRITE_SCALE);
    _body->setDelegate(this);
    GObjectData d;
    d.player = this;
    _body->setGObjectData(d);

    _collidingObject = NULL;

    _grounded = false;
    _onStairs = false;

    _hurt = false;
    _iHurt = 0;
    _iBeingHurt = 0;
}

Player::~Player(){

}

void Player::setupAnimations() {
    addAnimation(1, 0, 0, "Idle", 32, 32, Vector2());
    addAnimation(4, 0, 0, "Walk", 32, 32, Vector2());
    addAnimation(4, 5, 0, "Jump", 32, 32, Vector2());
    addAnimation(3, 11, 0, "Slash", 32, 32, Vector2());
    addAnimation(4, 18, 0, "Climb", 32, 32, Vector2());
}

void Player::animationDone(AnimatedSprite* s, std::string currentAnimation) {
    if(currentAnimation == "Jump" || currentAnimation == "Climb"){
        if(_boundingBox->getVelocity().x != 0)
            playAnimation("Walk");
        else
            playAnimation("Idle");
    }
}

void Player::gainHealth(int amount) {
    if(amount < 0 && !_hurt) {
        _hurt = true;
        SDL_SetTextureColorMod(_spriteSheet, 231, 76, 60);
    }
    else if(amount < 0)
        amount = 0;

    _currentHealth += amount;

    if(_currentHealth < 0){
        _currentHealth = 0;
    }
    else if(_currentHealth == 0 && _playerDelegate)
        _playerDelegate->playerIsDead(this);
}

void Player::revive(GPoint pos) {
    _body->setPosition(pos);
    gainHealth(3);

    SDL_SetTextureColorMod(_spriteSheet,  255, 255, 255);

    _iBeingHurt = 0;
    _iHurt      = 0;
    _hurt       = false;
}

void Player::moveLeft() {
    if(_body->getVelocity().x > -PlayerConstants::WALK_SPEED * (_running ? PlayerConstants::RUN_MULT : 1))
        _body->setVelocityX(-PlayerConstants::WALK_SPEED * (_running ? PlayerConstants::RUN_MULT : 1));

    if(_grounded && !_onStairs)
        playAnimation("Walk");

    _facing = LEFT;
}

void Player::moveRight() {
    if(_body->getVelocity().x < (PlayerConstants::WALK_SPEED * (_running ? PlayerConstants::RUN_MULT : 1)))
        _body->setVelocityX(PlayerConstants::WALK_SPEED * (_running ? PlayerConstants::RUN_MULT : 1));

    if(_grounded && !_onStairs)
        playAnimation("Walk");

    _facing = RIGHT;
}

void Player::stopMoving() {
    _body->setVelocityX(0);
    playAnimation("Idle");
}

void Player::jump() {
    if(_grounded){
        _grounded = false;
        playAnimation("Jump", true);
        float x = (_body->getVelocity().x == 0 ? 0.0f : (_facing == RIGHT ? 1.5f : -1.5f));
        if(_running)
            x *= PlayerConstants::RUN_MULT;
        _body->applyImpulse(GVector(x, PlayerConstants::JUMP_SPEED));
    }
}

void Player::moveUp() {
    if(_onStairs){
        float posY = _body->getPosition().y;
        _body->setPositionY(posY - PlayerConstants::WALK_SPEED * (_running ? PlayerConstants::RUN_MULT : 1));

        playAnimation("Climb");

        _facing = UP;
    }
}

void Player::moveDown() {
    if(_onStairs){
        float posY = _body->getPosition().y;
        _body->setPositionY(posY + PlayerConstants::WALK_SPEED * (_running ? PlayerConstants::RUN_MULT : 1));

        playAnimation("Climb");

        _facing = UP;
    }
}

void Player::update(float elapsedTime) {
    GVector adjustPosition;
    adjustPosition.x = -(_srcRect.w - _body->getSize().w / 2);
    adjustPosition.y = -(_srcRect.h - _body->getSize().h / 2) * 2;

    _x = _body->getPosition().x + adjustPosition.x;
    _y = _body->getPosition().y + adjustPosition.y;

    if(_onStairs && _body->getGravityScale() != 0)
        _body->setGravityScale(0.0f);
    else if(!_onStairs && _body->getGravityScale() == 0){
        _body->setGravityScale(1.0f);
        if(_currentAnimation == "Climb")
            stopAnimation();
    }

    _grounded = _body->isGrounded();

    if(_hurt){
        _iBeingHurt += elapsedTime;
        _iHurt++;

        if(_iBeingHurt > PlayerConstants::MAX_TIME_HURT){
            SDL_SetTextureColorMod( _spriteSheet,  255, 255, 255);

            _iBeingHurt = 0;
            _iHurt      = -PlayerConstants::HURT_TIME_SWITCH;
            _hurt       = false;
        }
    }

    AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics) {
    if(!_hurt || (_iHurt  > 0 && _iHurt < PlayerConstants::HURT_TIME_SWITCH))
        AnimatedSprite::draw(graphics, _x, _y,  0, NULL, (_facing == RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL));
    else if(_hurt && _iHurt >= PlayerConstants::HURT_TIME_SWITCH)
        _iHurt = -PlayerConstants::HURT_TIME_SWITCH;

    if(!_body->getLastMTD().isNull()){

        SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 255, 0, 255);
        SDL_RenderDrawLine(graphics.getRenderer(),
            _body->getPosition().x - graphics.getCamera().getPosition().x,
            _body->getPosition().y - graphics.getCamera().getPosition().y,
                           _body->getPosition().x + _body->getLastMTD().x - graphics.getCamera().getPosition().x,
                           _body->getPosition().y + _body->getLastMTD().y - graphics.getCamera().getPosition().y
        );
    }
}

void Player::touchActiveTile(ActiveTile *tile, Sides::Side side) {
    tile->getBBox()->setGrounded(false);

    switch(side){
        case Sides::TOP:
            _body->setPositionY(tile->getBBox()->getPosition().y + tile->getBBox()->getBBox().size.h);
            _body->setVelocityY(0);
            break;
        case Sides::BOTTOM:
            _body->setPositionY(tile->getBBox()->getPosition().y - _body->getBBox().size.h);
            _body->setGrounded(true);
            break;
        case Sides::LEFT:
            if(tile->getBBox()->isBlockedLeft()){
                //std::cout << "BLOCKED" << std::endl;
                _body->setPositionX(tile->getBBox()->getPosition().x + tile->getBBox()->getBBox().size.w);
            }
            else{
                //std::cout << "NOT BLOCKED" << std::endl;
                tile->setPushed(true);
                tile->getBBox()->applyImpulse(GVector(-1, 0));
            }
            break;
        case Sides::RIGHT:
            if(tile->getBBox()->isBlockedRight()){
                _body->setPositionX(tile->getBBox()->getPosition().x - _body->getBBox().size.w);
            }
            else{
                tile->setPushed(true);
                tile->getBBox()->applyImpulse(GVector(1, 0));
            }
            break;
        default:
            break;
    }
}

void Player::touchTile(Tile *tile, Sides::Side side) {
    switch(side){
        case Sides::TOP:
            _body->setPositionY(tile->getBBox()->getPosition().y + tile->getBBox()->getBBox().size.h);
            _body->setVelocityY(0);
            break;
        case Sides::BOTTOM:
            _body->setPositionY(tile->getBBox()->getPosition().y - _body->getBBox().size.h);
            _body->setGrounded(true);
            break;
        case Sides::LEFT:
            _body->setPositionX(tile->getBBox()->getPosition().x + tile->getBBox()->getBBox().size.w);
            break;
        case Sides::RIGHT:
            _body->setPositionX(tile->getBBox()->getPosition().x - _body->getBBox().size.w);
            break;
        default:
            break;
    }
}

void Player::setOnStairs(bool o) {
    _onStairs = o;
    _body->setGrounded(o);
}