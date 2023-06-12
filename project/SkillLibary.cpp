#include "SkillLibary.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include "Common.h"

map<string, Skill> SkillLibray::skillLibray;

void SkillLibray::loadFromFile(const string& fileName)
{
	ifstream fin(fileName);
	if (!fin)
	{
		cout << "Can't open file: " << fileName << endl;
		return;
	}
	string name;
	TYPE type;
	SKILL_TYPE skillType;
	int power;
	int accuracy;
	int PP;
	EFFECT_NAME effect;

	string line;

	while (getline(fin, line)) {
		stringstream ss;
		string temp;

		ss << line;

		// 輸入Name
		ss >> name;

		// 輸入type
		ss >> temp;
		type = strToType(temp);

		// 輸入skill type
		ss >> temp;
		if (temp == "Physical") {
			skillType = SKILL_TYPE::PHYSICAL;
		}
		else if (temp == "Special") {
			skillType = SKILL_TYPE::SPECIAL;
		}

		// 輸入power
		ss >> power;

		// 輸入accuracy
		ss >> accuracy;

		// 輸入PP
		ss >> PP;

		// 輸入effect
		ss >> temp;


		if (temp == "") {
			effect = EFFECT_NAME::NONE;
		}
		else if (temp == "PARALYSIS") {
			effect = EFFECT_NAME::PARALYSIS;
		}
		else if (temp == "BURN") {
			effect = EFFECT_NAME::BURN;
		}
		else if (temp == "POISON") {
			effect = EFFECT_NAME::POISON;
		}

		Skill skill = { name, power, accuracy, PP, type, skillType, effect };

		cout << skill.PP;

		skillLibray[name] = skill;
	}

	fin.close();
}

Skill SkillLibray::getSkill(const string& name)
{
	return skillLibray[name];
}