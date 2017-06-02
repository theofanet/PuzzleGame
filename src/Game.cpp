//
// Created by Theo Fanet on 22/10/2016.
//

#include <SDL2/SDL.h>
#ifdef __WIN32__
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include <sstream>
#include <iomanip>
#include <Game.h>
#include <Input.h>

namespace {
    const int FPS = 60;
}

//Game constructor, Init and lunch the main loop
Game::Game(std::string winName) {
    _player = NULL;
    _level  = NULL;
    _nextLevel = NULL;
    _activeDoor = NULL;

    _windowName = winName;

    _currentTime  = 0;
    _previousTime = 0;

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    if(TTF_Init() == -1)
        std::cout << "TTF_Init Error : " << TTF_GetError() << std::endl;

    gameLoop();
}

//Game Destructor
Game::~Game() {
    delete_obj(_player);
    delete_obj(_level);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

//Main loop of the game, this is where all happens
void Game::gameLoop() {
    Input input;
    SDL_Event event;
    Graphics g;

    _graphics = g;
    _graphics.setWindowTilte(_windowName);

    _player = new Player(_graphics, Vector2());

    _level = new Level(_graphics, _player,  "Simple");
    _level->setDelegate(this);

    _hud = HUD(_graphics, _player);

    while(true){
        input.beginNewFrame();

        if(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                if(event.key.repeat == 0)
                    input.keyDownEvent(event);
            }
            else if(event.type == SDL_KEYUP)
                input.keyUpEvent(event);
            else if(event.type == SDL_QUIT)
                return;
        }

        if(input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
            return;

        _player->setRunning(false);
        if(input.isKeyHeld(SDL_SCANCODE_LSHIFT))
            _player->setRunning();

        if(input.isKeyHeld(SDL_SCANCODE_LEFT))
            _player->moveLeft();
        else if(input.isKeyHeld(SDL_SCANCODE_RIGHT))
            _player->moveRight();

        if(input.wasKeyPressed(SDL_SCANCODE_SPACE))
            _player->jump();

        if(input.wasKeyReleased(SDL_SCANCODE_RIGHT) || input.wasKeyReleased(SDL_SCANCODE_LEFT))
            _player->stopMoving();

        if(input.wasKeyPressed(SDL_SCANCODE_D))
            _level->toggleDrawDebug();
        if(input.wasKeyPressed(SDL_SCANCODE_R)){
            _player->getBody()->setVelocity(GVector());
            _player->getBody()->setPosition(GPoint(_level->getPlayerSpawnPoint().x, _level->getPlayerSpawnPoint().y));
        }

        if(input.wasKeyPressed(SDL_SCANCODE_UP) && _activeDoor){
            if(_activeDoor){
                /**
                 * Fix for second door of second map
                 * Map 3 not created for now and
                 * keys arn't implemented
                 * TODO : Add the keys system, Create Map 3
                 */
                if(_activeDoor->getDestination() != "Map 3"){
                    _activeDoor->setDelegate(this);
                    _activeDoor->playAnimation("Open", true);
                }
            }
        }
        else if(input.isKeyHeld(SDL_SCANCODE_UP))
            _player->moveUp();
        else if(input.isKeyHeld(SDL_SCANCODE_DOWN))
            _player->moveDown();

        _currentTime = SDL_GetTicks();
        int timeInterval = _currentTime - _previousTime;
        _previousTime = _currentTime;

        if(_player->isOnStairs())
            _player->setOnStairs(false);

        update((float)timeInterval);

        draw(_graphics);

        if(timeInterval < (1000 / FPS))
            SDL_Delay((Uint32)(1000 / FPS) - timeInterval);

        if(_nextLevel){
            delete_obj(_level);
            _level = _nextLevel;
            _nextLevel = NULL;
        }
    }
}

void Game::draw(Graphics &graphics) {
    SDL_SetRenderDrawColor(_graphics.getRenderer(), 39, 32, 52, 255);

    _graphics.clear();

    _level->draw(_graphics);

    _hud.draw(graphics);

    _graphics.flip();
}

void Game::update(float elapsedTime) {
    _activeDoor = NULL;

    _level->update(elapsedTime);
    _hud.update(elapsedTime);
}

void Game::playerReachDoor(Door *d){
    _activeDoor = d;
}

void Game::animationDone(AnimatedSprite *s, std::string currentAnimation) {
    if(s == _activeDoor && currentAnimation == "Open"){
        _nextLevel = new Level(_graphics, _player, _activeDoor->getDestination());
        _nextLevel->setDrawDebug(_level->isDrawingDebug());
        _nextLevel->setDelegate(this);
    }
}

void Game::resetLevel(Level *level) {
    _nextLevel = new Level(_graphics, _player, level->getMapName());
    _nextLevel->setDrawDebug(level->isDrawingDebug());
    _nextLevel->setDelegate(this);
}