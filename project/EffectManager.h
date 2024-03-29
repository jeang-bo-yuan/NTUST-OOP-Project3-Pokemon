/*****************************************************************//**
 * File: EffectManager.h
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-06-12
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-06-12
 * Description: This class is to control effect
 *********************************************************************/
#pragma once
#include "Effect.h"
#include <vector>



class EffectManager
{
private:
	static std::vector<Effect> effects;
	static double parRate;
public:
	EffectManager() = default;
	~EffectManager() = default;
	static void addEffect(EFFECT_NAME name, Creature* target, int turn, bool humanAttack);
    static std::string getEffectStr(Creature* target);
	static void printEffect(Creature* target);
	static const string getEffectNameSmall(EFFECT_NAME name);

	static void useEffect(Creature* creature, int turn);
	static void setParRate(double rate);
	static void reset();
};
