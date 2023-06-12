#pragma once
#include "Player.h"
#include <map>
#include <vector>
#include <sstream>
#include "Skill.h"
#include "Creature.h"

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
    int turn;
    bool isTesting = false;
    std::stringstream log;
    double criticalRate = 0.3;
public:
    Game();

    void newGame();

    std::string nextRound();

    void loadPokemonLib(std::string path);
    void loadMoveLib(std::string path);
    void loadGameData(std::string path);

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
