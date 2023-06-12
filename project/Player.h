#pragma once
#include "Creature.h"
#include <vector>
#include "Object.h"

class Player
{
private:
	int currentCreatureIndex = 0;
public:
	Player();
	
    std::vector<Object*> objects;

<<<<<<< Updated upstream
    std::vector<Creature*> creatures;

    Creature* currentCreature;
=======
	std::vector<Creature> creatures;

	Creature& getCurrentCreature();

	void setCurrentCreature(int index);
>>>>>>> Stashed changes

	void reset();

};

