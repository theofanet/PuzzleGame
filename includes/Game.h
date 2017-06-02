//
// Created by Theo Fanet on 22/10/2016.
//

#ifndef TESTPROG_GAME_H
#define TESTPROG_GAME_H

#include "Entities/Player.h"
#include "Level.h"
#include "GGraph/HUD.h"
#include "GGraph/Graphics.h"
#include "GPhy/GWorld.h"
#include "Entities/Door.h"


class Game : public GCollisionDelegate, public LevelDelegate, public AnimatedSpriteDelegate{
public:
    Game(std::string winName = "");
    ~Game();

    void playerReachDoor(Door *d);
    void resetLevel(Level *level);

    void animationDone(AnimatedSprite *s, std::string currentAnimation);

private:
    void gameLoop();
    void draw(Graphics &graphics);
    void update(float elapsedTime);

    Player *_player;

    int _currentTime, _previousTime;

    Level  *_level;
    Level  *_nextLevel;
    HUD    _hud;

    Door *_activeDoor;

    std::string _windowName;

    //TEST FONT
    TTF_Font *_police = NULL;

    Graphics _graphics;
};

#endif //TESTPROG_GAME_H
