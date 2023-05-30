#include "Object.h"
#include "Creature.h"
#include <sstream>

using namespace std;


Object::Object()
{
}

Object::Object(std::string name)
	:name(name)
{
}

Object::Object(std::string name, int healVolume)
	:name(name),healVolume(healVolume)
{
}

string Object::effectToCreature(Creature* goal)
{
	if (usageCount > 0)
		usageCount--;

	stringstream result;

	int finalHealVolume;

    if(healVolume == -1 || healVolume > goal->getMaxHp() - goal->getHp())
	{
        finalHealVolume = goal->getMaxHp() - goal->getHp();
	}
	else
	{
		finalHealVolume = healVolume;
	}

	goal->heal(finalHealVolume);

	result<<goal->getName()<<" is healed by "<<finalHealVolume<<" points.\n";

	return result.str();
}

void Object::setName(std::string name)
{
	this->name = name;
}

std::string Object::getName() const
{
	return name;
}

void Object::setUsageCount(int usageCount)
{
	this->usageCount = usageCount;
}

int Object::getUsageCount() const
{
	return usageCount;
}

void Object::setHealVolume(int healVolume)
{
	this->healVolume = healVolume;
}

std::string Object::getObjectDescription() const
{
	return objectDescription;
}

void Object::setObjectDescription(std::string objectDescription)
{
	this->objectDescription = objectDescription;
}
