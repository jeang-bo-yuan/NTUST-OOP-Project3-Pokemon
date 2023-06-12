#pragma once
#include "Creature.h"
#include <vector>
#include "Object.h"

class Player
{
private:
	std::vector<Object> objects;
	std::vector<Creature> creatures;
	int currentCreatureIndex;
public:
	Player();
	
	void loadFromFile(istream& in);

	const Creature& getCurrentCreature() const;
	Creature& getCurrentCreature();
	const Creature& getCreature(int index) const;
	Creature& getCreature(int index);
	const Object& getObject(int index) const;
	void switchCurrentCreature(int index);
	void addCreature(Creature creature);
	void addObject(Object object);


	int objectsSize() const;
	int creaturesSize() const;

	void reset();

	friend istream& operator>>(istream& is, Player& player);

};

