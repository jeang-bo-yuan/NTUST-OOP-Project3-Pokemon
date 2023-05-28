#include "Player.h"

Player::Player()
{
}

void Player::reset()
{
	for (auto& c : creatures)
		delete c;
	creatures.clear();
}

