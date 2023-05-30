
#include "Creature.h"

using namespace std;

istream& operator>>(istream& input,skill& goal)
{
    std::string type;

    input>>goal.name>>type;

    for(auto& c:type)
    {
        if(c>='A' && c<='Z')
            c+=0x20;
    }

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

Creature::Creature()
{

}

void Creature::getSkill(skill source)
{
    double damage = (2*source.level+10)/250*source.power;

    if(source.isPhysical)
        damage= damage*source.atk/def+2;
    else
        damage = damage*source.atk/spDef+2;

    damage *= source.magnification;

    hp -= damage;

    if(hp<0)
        hp = 0;
}

istream& operator>>(istream&  input,Creature& goal)
{
    int count;

    input>>count;

    for(int i=0;i<count;i++)
    {
        string typeName;

        input>>typeName;
        goal.types.push_back(typeName);
    }

    input>>goal.maxHp>>goal.atk>>goal.def>>goal.spAtk>>goal.spDef>>goal.speed;

    goal.hp = goal.maxHp;

    return input;
}

int Creature::getHp()
{
    return hp;
}

int Creature::getMaxHp()
{
    return maxHp;
}

int Creature::getSpeed()
{
    return speed;
}
int Creature::getAtk()
{
    return atk;
}
int Creature::getDef()
{
    return def;
}
int Creature::getSpAtk()
{
    return spAtk;
}
int Creature::getSpDef()
{
    return spDef;
}

string Creature::getName()
{
    return name;
}

void Creature::heal(int volume)
{
    hp+=volume;
}
