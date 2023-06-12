/*****************************************************************//**
 * File: Common.cpp
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-06-12
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-06-12
 * Description: common enum and function
 *********************************************************************/
#include "Common.h"	
#include <string>

using namespace std;

// Intent: 把string轉成type
// Pre: str是一個合法的type字串
// Post: 回傳對應的type
TYPE strToType(const string& str) {
	if (str == "Normal") return TYPE::NORMAL;
	if (str == "Fire") return TYPE::FIRE;
	if (str == "Water") return TYPE::WATER;
	if (str == "Electric") return TYPE::ELECTRIC;
	if (str == "Grass") return TYPE::GRASS;
	if (str == "Ice") return TYPE::ICE;
	if (str == "Fighting") return TYPE::FIGHTING;
	if (str == "Poison") return TYPE::POISON;
	if (str == "Ground") return TYPE::GROUND;
	if (str == "Flying") return TYPE::FLYING;
	if (str == "Psychic") return TYPE::PSYCHIC;
	if (str == "Bug") return TYPE::BUG;
	if (str == "Rock") return TYPE::ROCK;
	if (str == "Ghost") return TYPE::GHOST;
	if (str == "Dragon") return TYPE::DRAGON;
	if (str == "Dark") return TYPE::DARK;
	if (str == "Steel") return TYPE::STEEL;
	if (str == "Fairy") return TYPE::FAIRY;
	return TYPE::NORMAL;
}

const std::string typeToStr(TYPE type)
{
	if (type == TYPE::NORMAL) return "Normal";
	if (type == TYPE::FIRE) return "Fire";
	if (type == TYPE::WATER) return "Water";
	if (type == TYPE::ELECTRIC) return "Electric";
	if (type == TYPE::GRASS) return "Grass";
	if (type == TYPE::ICE) return "Ice";
	if (type == TYPE::FIGHTING) return "Fighting";
	if (type == TYPE::POISON) return "Poison";
	if (type == TYPE::GROUND) return "Ground";
	if (type == TYPE::FLYING) return "Flying";
	if (type == TYPE::PSYCHIC) return "Psychic";
	if (type == TYPE::BUG) return "Bug";
	if (type == TYPE::ROCK) return "Rock";
	if (type == TYPE::GHOST) return "Ghost";
	if (type == TYPE::DRAGON) return "Dragon";
	if (type == TYPE::DARK) return "Dark";
	if (type == TYPE::STEEL) return "Steel";
	if (type == TYPE::FAIRY) return "Fairy";
	return "Normal";
}