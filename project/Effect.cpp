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
			std::cout << target->getName() <<  " is hurt by its burn!" << std::endl;
			break;
		case EFFECT_NAME::POISON:
			target->beRealDamange(target->getMaxHp() / 16);// 造成生命1/16的傷害
			std::cout << target->getName() << " is hurt by its poisoning!" << std::endl;
			break;
	}
}
