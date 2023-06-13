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
#include <random>
#include "Creature.h"

std::vector<Effect> EffectManager::effects;
double EffectManager::parRate;

// Intent: 添加Effect到Creatue上
// Pre: name: Effect名稱, target: 要添加Effect的Creature
// Post: 將Effect添加到Creature上
void EffectManager::addEffect(EFFECT_NAME name, Creature* target, int turn, bool humanAttack)
{
	effects.push_back(Effect(name, target));

	// 如果是麻痺就降低target的速度
	if (name == EFFECT_NAME::PARALYSIS) {
		target->decreaseSpeed();

		cout << "[Turn " << turn << " ] ";
		if (humanAttack) {
			cout << "The opposing ";
		}
		cout << target->getName() << " is paralyzed, so it may be unable to move!" << endl;
	}
	else {
		// Print info
		cout << "[Turn " << turn << " ] ";
		if (humanAttack) {
			cout << "The opposing ";
		}
		cout << target->getName() << " was " << EffectManager::getEffectNameSmall(name) << "ed!" << endl;
	}
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

// Intent: 印出Creature所擁有的Effect
// Pre: target: 要印出Effect的Creature
// Post: 印出Creature所擁有的Effect
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

// Intent: 觸發creature所擁有的所有effect
void EffectManager::useEffect(Creature* creature, int turn)
{
	for (auto& effect : effects) {
		if (effect.getCreature() == creature) {
			if (effect.getName() == "BRN" || effect.getName() == "PSN") {
				cout << "[Turn " << turn << "] " << effect.getCreature()->getName() << " is hurt by its ";

				cout << getEffectNameSmall(effect.getEffectName());

				cout << "!" << endl;

				effect.use();
			}
			else if (effect.getName() == "PAR") {
				std::random_device rd;
				std::mt19937 gen(rd());

				std::uniform_real_distribution<double> dis(0.0, 1.0);

				// 生成随机数
				double randomNum = dis(gen);

				if (randomNum <= parRate) {
					creature->setParalyzed(true);
				}
				else {
					creature->setParalyzed(false);
				}
			}
		}
	}
}

void EffectManager::setParRate(double rate)
{
    parRate = rate;
}

// Intetn: 重置Effect
// Pre: None
// Post: 重置Effect
void EffectManager::reset()
{
	effects.clear();
}
