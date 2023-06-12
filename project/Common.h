/*****************************************************************//**
 * File: Common.h
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-06-12
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-06-12
 * Description: common header file
 *********************************************************************/
#pragma once
#include <iostream>

enum class TYPE {
    NORMAL = 0, FIRE, WATER, ELECTRIC,
    GRASS, ICE, FIGHTING,
    POISON, GROUND, FLYING,
    PSYCHIC, BUG, ROCK, GHOST,
    DRAGON, DARK, STEEL, FAIRY
};


TYPE strToType(const std::string& str);
const std::string typeToStr(TYPE type);
