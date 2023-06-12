#include "Player.h"
#include "CreatureLibary.h"
#include "SkillLibary.h"

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

void Player::switchCurrentCreature(int index)
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

int Player::objectsSize() const
{
	return objects.size();
}

int Player::creaturesSize() const
{
	return creatures.size();
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
