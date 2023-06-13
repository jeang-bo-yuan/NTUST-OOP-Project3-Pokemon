/*****************************************************************//**
 * File: Effect.cpp
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-06-12
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-06-12
 * Description: This progress is implention effect
 *********************************************************************/
#include "Effect.h"
#include <iostream>
#include "Creature.h"


Effect::Effect(EFFECT_NAME name, Creature* target) : name(name), target(target)
{
}

void Effect::use()
{
	switch (name) {
	case EFFECT_NAME::PARALYSIS:
		// target->setParalysis(true);
		break;
	case EFFECT_NAME::BURN:
		target->beRealDamange(target->getMaxHp() / 16); // 造成生命1/16的傷害
		break;
	case EFFECT_NAME::POISON:
		target->beRealDamange(target->getMaxHp() / 16);// 造成生命1/16的傷害
		break;
	}
}

Creature* Effect::getCreature() const
{
	return target;
}

const string Effect::getName() const
{
	switch (name) {
	case EFFECT_NAME::PARALYSIS:
		return "PAR";
		break;
	case EFFECT_NAME::BURN:
		return "BRN";
		break;
	case EFFECT_NAME::POISON:
		return "PSN";
		break;
	}
}

bool Effect::isNone() const
{
	if (name == EFFECT_NAME::NONE) {
		return true;
	}
	else {
		return false;
	}
}
