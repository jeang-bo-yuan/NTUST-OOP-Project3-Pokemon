#include "Player.h"

Player::Player()
{
}

Creature& Player::getCurrentCreature()
{
	return creatures[currentCreatureIndex];
}

void Player::setCurrentCreature(int index)
{
	currentCreatureIndex = index;
}

void Player::reset()
{
<<<<<<< Updated upstream
    for(auto o:objects)
        delete o;

    for(auto c:creatures)
        delete c;

	creatures.clear();
    objects.clear();
=======
>>>>>>> Stashed changes
}

