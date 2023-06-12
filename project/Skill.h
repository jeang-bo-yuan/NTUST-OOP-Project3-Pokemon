
#ifndef SKILL_H
#define SKILL_H
#include <string>
#include <iostream>

void sToLower(std::string& str);


typedef struct Skill
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
    double magnification;
}skill;

std::istream& operator>>(std::istream& input,Skill& goal);


#endif // SKILL_H
