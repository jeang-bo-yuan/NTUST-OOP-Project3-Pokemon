/*****************************************************************//**
 * File: Creature.h
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-06-12
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-06-12
 * Description: the 寶可夢 
 *********************************************************************/
#pragma once

#include <string>
#include <vector>
#include <iostream>
<<<<<<< Updated upstream
#include "Skill.h"
=======
#include "Common.h"
#include "SkillLibray.h"

using namespace std;

double getTypeRate(TYPE attack, TYPE defend);
>>>>>>> Stashed changes

class Creature
{
private:
    string name;

    vector<TYPE> types;
    vector<Skill>skills;

    int hp;
    int maxHp;
    int atk;
    int def;
    int spAtk;
    int spDef;
    int speed;
    const int level = 50;
    double dodgeRate = 0.3;
public:
    friend class CreatureLibray;
    Creature();
<<<<<<< Updated upstream
    void getSkill(Skill source);
=======
    Creature(const Creature& creature);

    void useSkill(int index, Creature* target);

    double getDodgeRate();

    void beRealDamange(int value);

    const std::string& getName();
    void heal(int volume);

    bool isSameType(TYPE type);

    friend istream& operator>>(istream& is, Creature& creature);

    Creature& operator=(const Creature& creature);

    // Accessor
>>>>>>> Stashed changes
    int getHp();
    int getMaxHp();
    int getSpeed();
    int getAtk();
    int getDef();
    int getSpAtk();
    int getSpDef();
<<<<<<< Updated upstream
    double getDodgeRate();
    std::string getName();
    void heal(int volume);

    friend std::istream& operator>>(std::istream& input,Creature& goal);

public:
    std::vector<std::string> types;
    std::vector<Skill>skills;
    std::vector<std::string> conditions;
=======
    int getSkillSize() const;
    string getSkillName(int index) const;
    Skill getSkill(int index) const;
    int getTypeSize() const;
    const string& getTypeName(int index) const;
    void addSkill(Skill skill);
>>>>>>> Stashed changes
};

