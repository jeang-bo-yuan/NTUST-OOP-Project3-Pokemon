#pragma once
#include "Player.h"
#include <map>
#include <vector>
#include <sstream>
#include "Creature.h"
#include "SkillLibary.h"
#include "CreatureLibary.h"
#include "GameControl.h"

class Game
{
private:
    const int humanIndex = 0;
    Player player[2];
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

    void setTesting();

    std::string useObject(Object* object, class Creature* goal);

    void useSkill(int skillIndex, class Creature& goal); // 怪物使用技能
    void swapCreature(int creatureIndex); // 怪物交換

    void humanAttack(int index);

    void swapTurn();

    Player* getCurrentPlayer();
    Player* getNotCurrentPlayer();


public:
    std::vector<Skill> moveLib;
    std::vector<Creature> pokemonLib;
};
