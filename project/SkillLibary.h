#pragma once
#include "Effect.h"
#include <map>
#include <string>
#include <iostream>
#include "Common.h"

using namespace std;

enum SKILL_TYPE {
	SPECIAL,
	PHYSICAL,
	STATUS
};

struct Skill
{
	string name;
	int power;
	int accuracy; // 0 ~ 100
	int PP;
	TYPE type;
	SKILL_TYPE skillType;
	EFFECT_NAME effect;
};

class SkillLibray
{
private:
	static map<string, Skill> skillLibray;

public:
	static void loadFromFile(const string& filename);
	static Skill getSkill(const string& name);
};