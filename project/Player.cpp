#include "Player.h"
#include "CreatureLibary.h"
#include "SkillLibary.h"
#include <algorithm>

Player::Player()
{
	currentCreatureIndex = 0;
}

const Creature& Player::getCurrentCreature() const
{
	return creatures[currentCreatureIndex];
}

Creature& Player::getCurrentCreature()
{
	return creatures[currentCreatureIndex];
}

const Creature& Player::getCreature(int index) const
{
	return creatures[index];
}

Creature& Player::getCreature(int index)
{
	return creatures[index];
}

const Object& Player::getObject(int index) const
{
    return objects[index];
}

Object &Player::getObject(int index)
{
    return objects[index];
}

void Player::swapCreature(int index)
{
	currentCreatureIndex = index;
}

void Player::addCreature(Creature creature)
{
	creatures.push_back(creature);
}

void Player::addObject(Object object)
{
	objects.push_back(object);
}

void Player::useObject(const string& creatureName, const string& objectName)
{
}

void Player::swapCreature(const string& creatureName, int turn)
{
	int index = -1;
	for (int i = 0; i < creatures.size(); i++) {
		if (creatures[i].getName() == creatureName) {
			index = i;
		}
	}

	if (index == -1) {
		cout << "Not find the creature " << creatureName << endl;
	}

	this->swapCreature(index);
}

int Player::findCreatureIndex(const string& creatureName) const
{
	for (int i = 0; i < creatures.size(); i++) {
		if (creatures[i].getName() == creatureName) {
			return i;
		}
	}

	return -1; // not find
}

int Player::objectsSize() const
{
	return objects.size();
}

int Player::creaturesSize() const
{
    return creatures.size();
}

bool Player::isAlive() const
{
    return std::any_of(creatures.begin(), creatures.end(), [](const Creature& c)->bool {
        return c.getHp() > 0;
    });
}

void Player::reset()
{
	creatures.clear();
    objects.clear();
}

istream& operator>>(istream& is, Player& player)
{
	int creatureNum;

	is >> creatureNum;

	for (int i = 0; i < creatureNum; i++) {
		string creatureName;
		int skillNum;

		is >> creatureName;

		Creature creature(CreatureLibray::getCreature(creatureName));

		is >> skillNum;

		for (int i = 0; i < skillNum; i++) {
			string skillName;

			is >> skillName;

			creature.addSkill(SkillLibray::getSkill(skillName));
		}

		player.addCreature(creature);
	}

	return is;
}
