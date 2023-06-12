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

public:
	EffectManager() = default;
	~EffectManager() = default;
	static void addEffect(EFFECT_NAME name, Creature* target);
	static void useEffect();
};
