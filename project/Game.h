#pragma once
#include "Player.h"
#include <map>
#include <vector>
#include <sstream>
#include "Creature.h"
#include "SkillLibary.h"
#include "CreatureLibary.h"

typedef struct skillInfo
{
    bool isAccuracy;
    short effectiveType;
    bool isCritical;

}skillInfo;

class Game
{
    Player player1;
    Player player2;

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

    std::string useSkill(int skillIndex, class Creature* goal);

    int getTurn();

public:
    Player* currentPlayer;
    Player* opponentPlayer;
    std::vector<Skill> moveLib;
    std::vector<Creature> pokemonLib;
};
