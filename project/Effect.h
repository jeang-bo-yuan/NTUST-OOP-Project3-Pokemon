/*****************************************************************//**
 * File: Effect.h
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-06-12
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-06-12
 * Description: This is a effect class
 *********************************************************************/
#pragma once
#include <string>
#include "Common.h"

using std::string;

class Creature;

enum class EFFECT_NAME {
	PARALYSIS,
	BURN,
	POISON,
	NONE,
};

class Effect
{
private:
	EFFECT_NAME name;
	Creature* target;

public:
	Effect(EFFECT_NAME name, Creature* target);
	~Effect() = default;
	void use();

	Creature* getCreature() const;
	const string getName() const;
	const EFFECT_NAME getEffectName() const { return name; }
	bool isNone() const;

};
