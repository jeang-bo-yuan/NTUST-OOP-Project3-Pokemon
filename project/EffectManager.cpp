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
}

// Intent: 使用Effect
// Pre: None
// Post: 使用Effect
void EffectManager::useEffect(int turn)
{
	for (auto& effect : effects) {
		cout << "[Turn " << turn << "] " << effect.getCreature()->getName() << " is hurt by its ";
		effect.use();
	}
}

void EffectManager::printEffect(Creature* target)
{
	for (const auto& i : effects) {
		if (i.getCreature() == target && !i.isNone()) {
			cout << " " << i.getName();
		}
	}
}

const string EffectManager::getEffectNameSmall(EFFECT_NAME name)
{
	switch (name) {
	case EFFECT_NAME::PARALYSIS:
		return "paralysis";
		break;
	case EFFECT_NAME::BURN:
		return "burn";
		break;
	case EFFECT_NAME::POISON:
		return "poison";
		break;
	case EFFECT_NAME::NONE:
		return "none";
		break;
	}

	return "unKnow";
}
