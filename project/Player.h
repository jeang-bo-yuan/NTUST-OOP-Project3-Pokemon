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
	void swapCreature(int index);
	void addCreature(Creature creature);
	void addObject(Object object);

	void useObject(const string& creatureName, const string& objectName);
	void swapCreature(const string& creatureName, int turn);


	int findCreatureIndex(const string& creatureName) const;

	int objectsSize() const;
	int creaturesSize() const;

	void reset();

	friend istream& operator>>(istream& is, Player& player);

};

