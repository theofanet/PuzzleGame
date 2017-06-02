//
// Created by Theo on 09/11/2016.
//

#ifndef PUZZELGAME_HUD_H
#define PUZZELGAME_HUD_H

#include "GGraph/Sprite.h"
#include <vector>

class Graphics;
class Player;


class HUD {
public:
    HUD(){}
    HUD(Graphics &graphics, Player* player);

    void draw(Graphics &graphics);
    void update(float elapsedTime);

private:
    Player *_player;

    //Health sprites
    Sprite _healthBarSprite;
    Sprite _healthNumber1;
    Sprite _currentHeathBar;

    //Exp sprites
    Sprite _lvWorld;
    Sprite _lvNumer;
    Sprite _expBar;

    std::vector<Sprite> _sprites;
};


#endif //PUZZELGAME_HUD_H
