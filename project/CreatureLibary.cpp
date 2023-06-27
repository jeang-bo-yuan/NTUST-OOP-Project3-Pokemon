#include "CreatureLibary.h"

std::map<string, Creature> CreatureLibray::creatures;

void CreatureLibray::loadFromFile(const std::string& fileName)
{

    ifstream in(fileName);
    string name;


    if (!in)
    {
        cerr << "Can't open file: " << fileName << endl;
        throw 1;
    }

    while (in >> name) {
        Creature creature;

        creature.name = name;

        int typeSize;

        in >> typeSize;

        for (int i = 0; i < typeSize; i++) {
            string type;
            in >> type;
            creature.types.push_back(strToType(type));
        }

        in >> creature.maxHp >> creature.atk >> creature.def >> creature.spAtk >> creature.spDef >> creature.speed;
        creature.hp = creature.maxHp;

        creatures[creature.name] = creature;

    }


}

Creature CreatureLibray::getCreature(const std::string& name)
{
    return creatures[name];
}
