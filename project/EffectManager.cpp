/*****************************************************************//**
 * File: EffectManager.cpp
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-06-12
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-06-12
 * Description: This class is implention effect
 *********************************************************************/
#include "EffectManager.h"
#include <iostream>
#include "Creature.h"

std::vector<Effect> EffectManager::effects;

// Intent: 添加Effect到Creatue上
// Pre: name: Effect名稱, target: 要添加Effect的Creature
// Post: 將Effect添加到Creature上
void EffectManager::addEffect(EFFECT_NAME name, Creature* target)
{
	effects.push_back(Effect(name, target));

	switch (name) {
		case EFFECT_NAME::PARALYSIS:
			std::cout << target->getName() << " is paralyzed, so it may be unable to move!" << std::endl;
			break;
		case EFFECT_NAME::BURN:
			std::cout << target->getName() << " was burned!" << std::endl;
			break;
		case EFFECT_NAME::POISON:
			std::cout << target->getName() << " was poisoned!" << std::endl;
			break;
	}
}

// Intent: 使用Effect
// Pre: None
// Post: 使用Effect
void EffectManager::useEffect()
{
	for (auto& effect : effects) {
		effect.use();
	}
}
