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
#include "Common.h"
#include "SkillLibary.h"


using namespace std;

double getTypeRate(TYPE attack, TYPE defend);



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

    bool paralyzed;
public:
    bool isHuman;

public:
    friend class CreatureLibray;
    Creature();

    Creature(const Creature & creature);

    void useSkill(int index, Creature& target, int turn, bool humanAttack);
    void useSkill(string skillName, Creature& target, int turn, bool humanAttack);

    void beRealDamange(int value);

    const std::string& getName() const;

    bool isSameType(TYPE type);

    friend istream& operator>>(istream& is, Creature& creature);

    Creature& operator=(const Creature& creature);
    void heal(int volume);

    void decreaseSpeed();
    void setParalyzed(bool can);

    // Accessor
    const string getPrintName() const;
    int getHp() const;
    int getMaxHp() const;
    int getSpeed() const;
    int getAtk() const;
    int getDef() const;
    int getSpAtk() const;
    int getSpDef() const;
    double getDodgeRate();
    double getDodgeRate() const;
    int getSkillSize() const;
    string getSkillName(int index) const;
    Skill getSkill(int index) const;
    int getTypeSize() const;
    const string getTypeName(int index) const;
    void addSkill(Skill skill);
};
