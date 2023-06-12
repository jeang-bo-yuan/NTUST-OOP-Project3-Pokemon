#pragma once
#include "Creature.h"
#include <vector>
#include "Object.h"

class Player
{
	
public:
	Player();
	
    std::vector<Object*> objects;

    std::vector<Creature*> creatures;

    Creature* currentCreature;

	void reset();

};

