#pragma once

#include <string>
#include <vector>
#include <iostream>


typedef struct skill
{
    std::string name;
    std::string type;
    int level = 50;
    bool isPhysical;
    int power;
    int accuracy;
    int pp;
    int atk;
    std::string condition;
    float magnification;
    friend std::istream& operator>>(std::istream& input,skill& goal);
}skill;

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
public:
    Creature();
    void getSkill(skill source);
    int getHp();
    int getMaxHp();
    int getSpeed();
    int getAtk();
    int getDef();
    int getSpAtk();
    int getSpDef();
    std::string getName();
    void heal(int volume);
    friend std::istream& operator>>(std::istream& input,Creature& goal);

public:
    std::vector<std::string> types;
    std::vector<skill>skills;
};

