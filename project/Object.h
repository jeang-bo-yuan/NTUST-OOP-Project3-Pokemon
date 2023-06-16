#pragma once
#include <string>

class Object
{
	int healVolume;
public:
	Object();

	Object(std::string name);

	Object(std::string name,int healVolume);

	virtual std::string effectToCreature(class Creature* goal);

	void setName(std::string name);

	std::string getName()const;
	const std::string getPrintName() const;

	void setUsageCount(int usageCount);

	int getUsageCount()const;

	void setHealVolume(int healVolume);

	std::string getObjectDescription()const;

	void setObjectDescription(std::string objectDescription);


protected:
	std::string name;

	int usageCount = -1;

	std::string objectDescription;

};
