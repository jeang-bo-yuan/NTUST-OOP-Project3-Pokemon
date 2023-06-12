#include "Game.h"
#include <random>

using namespace std;

Game::Game()
{
    player1 = new Player();
    player2 = new Player();

    Object* potion = new Object("Potion", 20);
    Object* superPotion = new Object("Super Potion", 60);
    Object* hyperPotion = new Object("Hyper Potion", 120);
    Object* maxPotion = new Object("Max Potion", -1);

    player1->objects.push_back(potion);
    player1->objects.push_back(superPotion);
    player1->objects.push_back(hyperPotion);
    player1->objects.push_back(maxPotion);
    player2->objects.push_back(potion);
    player2->objects.push_back(superPotion);
    player2->objects.push_back(hyperPotion);
    player2->objects.push_back(maxPotion);
}

void Game::newGame()
{
    isTesting = false;
}

std::string Game::nextRound()
{
    log.clear();
    return std::string();
}

void Game::setTesting()
{
    isTesting = true;
}

std::string Game::useObject(Object* object, Creature* goal)
{
    return object->effectToCreature(goal);
}

std::string Game::useSkill(int skillIndex, Creature* goal)
{
    srand(time(0));

    stringstream result;

    skill currentSkill = currentPlayer->currentCreature->skills[skillIndex];

    goal->skills[skillIndex].pp--;

    result<<"[Turn "<<turn<<"] "<<currentPlayer->currentCreature->getName()<<" used "<<currentSkill.name<<"!\n";

    if(!isTesting)
    {
        int i = rand()%100;

        if(i > currentSkill.accuracy)
        {
            result <<"[Turn "<<turn<<"] "<<goal->getName()<<" avoided the attack!\n";
            return result.str();
        }
    }

    currentSkill.magnification = 1;

    for(auto& type:currentPlayer->currentCreature->types)
    {
        if(type == currentSkill.type)
        {
            currentSkill.magnification *= 1.5;
            break;
        }
    }

    if(!isTesting)
    {
        int i = rand()%(100);

        if(i<= criticalRate*100)
        {
            currentSkill.magnification *= 1.5;
            result<<"[Turn "<<turn<<"] A critical hit!\n";
        }
    }

    double m=1;

    for(auto& type:goal->types)
    {

        m *= effectTable[effectIndex[currentSkill.type]][effectIndex[type]];
    }

    if(m>=2)
        result<<"[Turn "<<turn<<"] "<<"It's super effective!"<<"!\n";
    else if(m <= 0)
        result <<"[Turn "<<turn<<"] "<<"It's not effective!"<<"!\n";
    else if(m<=0.5)
        result <<"[Turn "<<turn<<"] "<<"It's not very effective!"<<"!\n";

    currentSkill.magnification *= m;

    if(!currentSkill.condition.empty())
    {
        int i = rand()%100;
        if(isTesting || i > goal->getDodgeRate()*100)
        {
            goal->conditions.push_back(currentSkill.condition);

            if(conditionId[currentSkill.condition] == 0)
            {
                result<<"[Turn "<<turn<<"] "<<goal->getName()<<" is paralyzed, so it may be unable to move!\n";
            }
            else if(conditionId[currentSkill.condition] == 1)
            {
                result<<"[Turn "<<turn<<"] "<<goal->getName()<<" was burned!\n";
            }
            else if(conditionId[currentSkill.condition] == 2)
            {
                result<<"[Turn "<<turn<<"] "<<goal->getName()<<" was poisoned!\n";
            }
        }
    }

    goal->getSkill(currentSkill);
    return result.str();
}

