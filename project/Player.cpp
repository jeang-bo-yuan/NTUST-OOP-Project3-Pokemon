#include "Player.h"

Player::Player()
{
}

void Player::reset()
{
    for(auto o:objects)
        delete o;

    for(auto c:creatures)
        delete c;

	creatures.clear();
    objects.clear();
}

