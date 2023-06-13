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

    // Intend: 判斷玩家能否繼續戰鬥
    // Pre: none
    // Post: true -> 若玩家任何一隻pokemon血量>0;
    //       false -> 全部pokemon血量==0
    bool isAlive() const;

	void reset();

	friend istream& operator>>(istream& is, Player& player);

};

