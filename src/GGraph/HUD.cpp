//
// Created by Theo on 09/11/2016.
//

#include "GGraph/HUD.h"
#include "GGraph/Graphics.h"
#include "Entities/Player.h"

#include <cmath>


HUD::HUD(Graphics &graphics, Player *player) {
    _player = player;

    _healthBarSprite = Sprite(graphics, "content/sprites/TextBox.png", 0, 40, 64, 8, 35, 70);
    _healthNumber1   = Sprite(graphics, "content/sprites/TextBox.png", 0, 56, 8, 8, 66, 70);
    _currentHeathBar = Sprite(graphics, "content/sprites/TextBox.png", 0, 25, 39, 5, 83, 72);

    _lvWorld = Sprite(graphics, "content/sprites/TextBox.png", 81, 81, 11, 7, 38, 55);
    _lvNumer = Sprite(graphics, "content/sprites/TextBox.png", 0, 56, 8, 8, 66, 52);
    _expBar  = Sprite(graphics, "content/sprites/TextBox.png", 0, 72, 40, 8, 83, 52);


    _sprites.push_back(_healthBarSprite);
    _sprites.push_back(_healthNumber1);
    _sprites.push_back(_currentHeathBar);
    _sprites.push_back(_lvWorld);
    _sprites.push_back(_lvNumer);
    _sprites.push_back(_expBar);
}

void HUD::update(float elapsedTime) {
    _sprites[1].setSourceRectX(8 * _player->getCurrentHealth());

    /**
     * Calculate the health bar 100% = 39px
     */
    float num = (float)_player->getCurrentHealth() / _player->getMaxHalth();
    _sprites[2].setSourceRectW(std::floor(num * 39));
}

void HUD::draw(Graphics &graphics) {
    for(std::vector<Sprite>::iterator it = _sprites.begin(); it != _sprites.end(); ++it)
        (*it).draw(graphics, (*it).getX() + graphics.getCamera().getPosition().x, (*it).getY() + graphics.getCamera().getPosition().y);
}