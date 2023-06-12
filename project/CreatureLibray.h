#pragma once
#include "Creature.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

class CreatureLibray
{

private:
	static std::map<string, Creature> creatures;

public:
	CreatureLibray() = default;
	~CreatureLibray() = default;
	static void loadFromFile(const std::string& filename);
	static Creature getCreature(const std::string& name);
};

