#include "Game.h"
#include <random>
#include <fstream>

using namespace std;


std::map<std::string,int> conditionId =
    {
        {"PAR",0},
        {"BRN",1},
        {"PSN",2}
};

std::map<std::string, int> effectIndex =
    {
        {"normal",0},
        {"fire",1},
        {"water",2},
        {"electric",3},
        {"grass",4},
        {"ice",5},
        {"fighting",6},
        {"poison",7},
        {"ground",8},
        {"flying",9},
        {"psychic",10},
        {"bug",11},
        {"rock",12},
        {"ghost",13},
        {"dragon",14},
        {"dark",15},
        {"steel",16},
        {"fairy",17}
};

std::vector<std::vector<float>> effectTable =
    {
        {1,1,1,1,1,1,1,1,1,1,1,1,0.5,0,1,1,0.5,1},
        {1,0.5,0.5,1,2,2,1,1,1,1,1,2,0.5,1,0.5,1,2,1},
        {1,2,0.5,1,0.5,1,1,1,2,1,1,1,2,1,0.5,1,1,1},
        {1,1,2,0.5,0.5,1,1,1,0,2,1,1,1,1,0.5,1,1,1},
        {1,0.5,2,1,0.5,1,1,0.5,2,0.5,1,0.5,2,1,0.5,1,0.5,1},
        {1,0.5,0.5,1,2,0.5,1,1,2,2,1,1,1,1,2,1,0.5,1},
        {2,1,1,1,1,2,1,0.5,1,0.5,0.5,0.5,2,0,1,2,2,0.5},
        {1,1,1,1,2,1,1,0.5,0.5,1,1,1,0.5,0.5,1,1,0,2},
        {1,2,1,2,0.5,1,1,2,1,0,1,0.5,2,1,1,1,2,1},
        {1,1,1,0.5,2,1,2,1,1,1,1,2,0.5,1,1,1,0.5,1},
        {1,1,1,1,1,1,2,2,1,1,0.5,1,1,1,1,0,0.5,1},
        {1,0.5,1,1,2,1,0.5,0.5,1,0.5,2,1,1,0.5,1,2,0.5,0.5},
        {1,2,1,1,1,2,0.5,1,0.5,2,1,2,1,1,1,1,0.5,1},
        {0,1,1,1,1,1,1,1,1,1,2,1,1,2,1,0.5,1,1},
        {1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,0.5,1,0},
        {1,1,1,1,1,1,0.5,1,1,1,2,1,1,2,1,0.5,0.5,0.5},
        {1,0.5,0.5,0.5,1,2,1,1,1,1,1,1,2,1,1,1,0.5,2}
};


Game::Game()
{
    newGame();
}

void Game::loadGameData(std::string path)
{
    ifstream input(path);

    int pNum = 0;

    input>>pNum;

    for(;pNum>0;pNum--)
    {
        string pName;
        int skillN=0;

        input>> pName;

        Creature* pokemon;

        for(auto p:pokemonLib)
        {
            if(p.getName() == pName)
            {
                pokemon = new Creature(p);

                break;
            }
        }

        input>>skillN;

        for(;skillN>0;skillN--)
        {
            string skillName;

            input>>skillName;

            for(auto skill:moveLib)
            {
                if(skill.name == skillName)
                {
                    pokemon->skills.push_back(skill);
                    break;
                }
            }
        }

        player1.creatures.push_back(pokemon);
    }

    player1.currentCreature = player1.creatures[0];

    input>>pNum;

    for(;pNum>0;pNum--)
    {
        string pName;
        int skillN=0;

        input>> pName;

        Creature* pokemon;

        for(auto p:pokemonLib)
        {
            if(p.getName() == pName)
            {
                pokemon = new Creature(p);

                break;
            }
        }

        input>>skillN;

        for(;skillN>0;skillN--)
        {
            string skillName;

            input>>skillName;

            for(auto skill:moveLib)
            {
                if(skill.name == skillName)
                {
                    pokemon->skills.push_back(skill);
                    break;
                }
            }
        }

        player2.creatures.push_back(pokemon);
    }

    player2.currentCreature = player2.creatures[0];
}

void Game::loadPokemonLib(std::string path)
{
    ifstream input(path);

    while(input)
    {
        Creature  pokemon;

        input>>pokemon;

        pokemonLib.push_back(pokemon);
    }
}

void Game::loadMoveLib(std::string path)
{
    ifstream input(path);

    while(input)
    {
        Skill skill;

        input>>skill;

        moveLib.push_back(skill);
    }
}

void Game::loadGame(std::string pokemonLibPath,std::string moveLibPath,std::string gameDataPath)
{
    newGame();

    loadPokemonLib(pokemonLibPath);
    loadMoveLib(moveLibPath);
    loadGameData(gameDataPath);

<<<<<<< Updated upstream
=======
    player1->objects.push_back(potion);
    player1->objects.push_back(superPotion);
    player1->objects.push_back(hyperPotion);
    player1->objects.push_back(maxPotion);
    player2->objects.push_back(potion);
    player2->objects.push_back(superPotion);
    player2->objects.push_back(hyperPotion);
    player2->objects.push_back(maxPotion);

    currentPlayer = player1;
    opponentPlayer = player2;
>>>>>>> Stashed changes
}

void Game::newGame()
{
    isTesting = false;
    player1 = Player();
    player2 = Player();
    currentPlayer = &player1;
    opponentPlayer = &player2;
    pokemonLib.clear();
    moveLib.clear();
    turn = 1;
    log.clear();

    Object potion("Potion", 20);
    Object superPotion("Super Potion", 60);
    Object hyperPotion("Hyper Potion", 120);
    Object maxPotion("Max Potion", -1);

    player1.objects.push_back(new Object(potion));
    player1.objects.push_back(new Object(superPotion));
    player1.objects.push_back(new Object(hyperPotion));
    player1.objects.push_back(new Object(maxPotion));
    player2.objects.push_back(new Object(potion));
    player2.objects.push_back(new Object(superPotion));
    player2.objects.push_back(new Object(hyperPotion));
    player2.objects.push_back(new Object(maxPotion));
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

<<<<<<< Updated upstream
    Creature* source = currentPlayer->currentCreature;

    source->skills[skillIndex].pp--;

    Skill currentSkill = source->skills[skillIndex];

    result<<"[Turn "<<turn<<"] "<<source->getName()<<" used "<<currentSkill.name<<"!\n";

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

    for(auto& type : source->types)
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
=======
    result << "[Turn "<< turn << "] "<< currentPlayer->getCurrentCreature().getName() << " used " << goal->getSkillName(skillIndex) << "!" << endl;
    currentPlayer->getCurrentCreature().useSkill(skillIndex, goal);

>>>>>>> Stashed changes
    return result.str();
}

