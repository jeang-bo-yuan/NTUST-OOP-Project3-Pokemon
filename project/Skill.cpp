
#include "Skill.h"

using namespace std;

void sToLower(string& str)
{
    for(auto& c:str)
    {
        if(c>='A' && c<='Z')
            c+=0x20;
    }
}

istream& operator>>(istream& input,Skill& goal)
{
    std::string type;

    input>>goal.name>>type;

    sToLower(type);

    goal.type = type;

    input>>type;

    if(type == "Physical")
        goal.isPhysical = true;
    else
        goal.isPhysical = false;

    input>>goal.power>>goal.accuracy>>goal.pp;

    if(input.peek() == ' ')
        input>>goal.condition;

    return input;
}
