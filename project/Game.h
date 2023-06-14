#pragma once
#include "Player.h"
#include <map>
#include <vector>
#include <sstream>
#include "Creature.h"
#include "SkillLibary.h"
#include "CreatureLibary.h"

class Game
{
private:
    const int humanIndex = 0;
    const int computerIndex = 1;
    Player player[2]; // 0: human, 1: computer
    int currentPlayerIndex;

    SkillLibray skillLib;
    CreatureLibray creatureLib;

    int turn;
    bool isTesting = false;
    std::stringstream log;
    double criticalRate = 0.3;
public:
    Game();

    void newGame();

    std::string nextRound();

    void loadGame(std::string pokemonLibPath,std::string moveLibPath,std::string gameDataPath);
    // Intend: load command file
    // Pre: none
    // Post return 1 if game is over; 0 otherwise
    int loadFromFile(const string& filename);

    void setTesting();
    void setTesting(bool test) { isTesting = test; }

    std::string useObject(Object* object, class Creature* goal);

    void useSkill(int skillIndex, class Creature& goal); // 怪物使用技能
    void useSkill(const string& skillName, class Creature& goal); // 怪物使用技能
    void swapCreature(int creatureIndex); // 怪物交換

    void humanAttack(int index);
    void computerAttack(int index);
    int getHumanCurrentCreatureHp() const;
    int getComputerCurrentCreatureHp() const;
    const string getHumanCurrentCreatureName() const;
    const string getComputerCurrentCreatureName() const;
    void printStatus();
    void printCheck();


    void swapTurn();

    Player* getCurrentPlayer();
    Player* getNotCurrentPlayer();


public:
    std::vector<Skill> moveLib;
    std::vector<Creature> pokemonLib;
};
