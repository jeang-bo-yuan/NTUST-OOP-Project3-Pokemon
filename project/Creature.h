#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "Skill.h"

class Creature
{
    std::string name;
    int hp;
    int maxHp;
    int atk;
    int def;
    int spAtk;
    int spDef;
    int speed;
    int level;
    double dodgeRate = 0.3;
public:
    Creature();
    void getSkill(Skill source);
    int getHp();
    int getMaxHp();
    int getSpeed();
    int getAtk();
    int getDef();
    int getSpAtk();
    int getSpDef();
    double getDodgeRate();
    std::string getName();
    void heal(int volume);

    friend std::istream& operator>>(std::istream& input,Creature& goal);

public:
    std::vector<std::string> types;
    std::vector<Skill>skills;
    std::vector<std::string> conditions;
};

