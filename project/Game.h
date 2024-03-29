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

private:
    void newGame();
public:
    Game();

    void nextRound_BandP(); // 下一回合 + 進入B&P階段 for GUI
    void nextTurn() { ++turn; }  // 將turn加一
    void loadGame(std::string pokemonLibPath,std::string moveLibPath,std::string gameDataPath); // 加載遊戲資料，檔案開啟失敗時throw 1
    int loadFromFile(const string& filename); // 從檔案載入遊戲資料 return 1 代表遊戲結束/ return 0 代表繼續，檔案開啟失敗時throw 1
    void setTesting(); // 設定測試模式
    void setTesting(bool test) { isTesting = test; } // 設定測試模式
    std::string useObject(Object* object, class Creature* goal); // 使用物品
    void swapCreature(int creatureIndex); // 怪物交換
    bool changeCreature(bool isComputer, int creatureIndex);
    void humanAttack(int index); // 玩家攻擊 index代表技能位置
    void computerAttack(int index); // 電腦攻擊 index代表技能位置
    bool isFainted(int playerIndex); // 判斷是否陣亡
    bool winOrLose(); // 判斷勝負

    void printStatus();
    void printCheck();

    // Auccessor
    int getHumanCurrentCreatureHp() const;
    int getComputerCurrentCreatureHp() const;
    const string getHumanCurrentCreatureName() const;
    const string getComputerCurrentCreatureName() const;
    Player* getCurrentPlayer();
    Player* getNotCurrentPlayer();


//public:
//    std::vector<Skill> moveLib;
//    std::vector<Creature> pokemonLib;
};
