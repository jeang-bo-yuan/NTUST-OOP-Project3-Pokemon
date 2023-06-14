#include "Game.h"
#include "EffectManager.h"
#include <random>
#include <fstream>
#include <QDebug>

#define TEST

using namespace std;


Game::Game()
{
    newGame();
}



void Game::loadGame(std::string pokemonLibPath,std::string moveLibPath,std::string gameDataPath)
{
    newGame();
#ifdef TEST
    ifstream gameData("D:/GitHub/Pokemon/project/test case/GameData.txt");
    skillLib.loadFromFile("D:/GitHub/Pokemon/project/test case/MoveLib.txt");
    creatureLib.loadFromFile("D:/GitHub/Pokemon/project/test case/PokemonLib.txt");
#else
     skillLib.loadFromFile(moveLibPath);
     creatureLib.loadFromFile(pokemonLibPath);
     ifstream gameData(gameDataPath);
#endif


    

    gameData >> player[0] >> player[1];

    player[0].setCreatureIsHuman(true);
    player[1].setCreatureIsHuman(false);
}

int Game::loadFromFile(const string& filename)
{
    string pokemonLibPath, moveLibPath, gameDataPath;
    string command;

#ifdef TEST
    ifstream file("D:/GitHub/Pokemon/project/test case/case2.txt");
#else
    ifstream file(filename);
#endif
    file >> pokemonLibPath >> moveLibPath >> gameDataPath;

    loadGame(pokemonLibPath, moveLibPath, gameDataPath);

    while (file >> command) {
        if (command == "Test") {
            isTesting = true;
            EffectManager::setParRate(1);
            continue;
        }
        else if (command == "Battle") {
            string skill1, skill2;

            file >> skill1 >> skill2;

            if (player[humanIndex].getCurrentCreature().getSpeed() >= player[computerIndex].getCurrentCreature().getSpeed()) {
                player[humanIndex].getCurrentCreature().useSkill(skill1, player[computerIndex].getCurrentCreature(), turn, true);
                if (player[computerIndex].getCurrentCreature().getHp() <= 0) goto END;
                player[computerIndex].getCurrentCreature().useSkill(skill2, player[humanIndex].getCurrentCreature(), turn, false);
			}
            else {
                player[computerIndex].getCurrentCreature().useSkill(skill2, player[humanIndex].getCurrentCreature(), turn, false);
                if (player[humanIndex].getCurrentCreature().getHp() <= 0) goto END;
                player[humanIndex].getCurrentCreature().useSkill(skill1, player[computerIndex].getCurrentCreature(), turn, true);
			}
        }
        else if (command == "Bag") {
            string potion, creatureName, computerSkill;

            file >> potion >> creatureName >> computerSkill;


            useObject(&player[humanIndex].getObject(potion), &player[humanIndex].getCreature(creatureName));

            player[computerIndex].getCurrentCreature().useSkill(computerSkill, player[humanIndex].getCurrentCreature(), turn, false);
        }
        else if (command == "Pokemon") {
            string creatureName, computerSkill;

            file >> creatureName >> computerSkill;

            int index = player[humanIndex].findCreatureIndex(creatureName);

            if (index == -1) {
                cout << "creature: " << creatureName << " not find!" << endl;
            }

            swapCreature(index);
            player[computerIndex].getCurrentCreature().useSkill(computerSkill, player[humanIndex].getCurrentCreature(), turn, false);

        }
        else if (command == "Status") {
            printStatus();
            continue;
        }
        else if (command == "Check") {
            printCheck();
            continue;
        }
        else if (command == "Run") {
            cout << "Player run away." << endl;
            return 1;
        }
        else {
            cout << "Unknow command!! " << " What is " << command << endl;
        }

        END:

        EffectManager::useEffect(&player[humanIndex].getCurrentCreature(), turn);
        EffectManager::useEffect(&player[computerIndex].getCurrentCreature(), turn);

        for (int index = 0; index <= 1; ++index)
            if (player[index].getCurrentCreature().getHp() <= 0) {
                // 全掛了
                if (!player[index].isAlive()) {
                    cout << "You " << (index == humanIndex ? "lose" : "win") << endl;
                    return 1;
                }

                for (int i = 0; i < player[index].creaturesSize(); ++i) {
                    if (&player[index].getCreature(i) == &player[index].getCurrentCreature()) {
                        player[index].swapCreature(i + 1);
                        std::cout << "[" << turn << "]" << (index == computerIndex ? "computer" : "human") << " swap fainted pokemon to " << player[index].getCurrentCreature().getName() << endl;
                        break;
                    }
                }
            }

        qDebug() << turn << player[humanIndex].getCurrentCreature().getName().c_str() << player[computerIndex].getCurrentCreature().getName().c_str();

        turn++;
    }

    file.close();
    currentPlayerIndex = humanIndex;
    return 0;
}

void Game::newGame()
{
    isTesting = false;
    player[0] = Player();
    player[1] = Player();
    currentPlayerIndex = 0;
    pokemonLib.clear();
    moveLib.clear();
    turn = 1;
    log.clear();
    EffectManager::reset();

    Object potion("Potion", 20);
    Object superPotion("SuperPotion", 60);
    Object hyperPotion("HyperPotion", 120);
    Object maxPotion("MaxPotion", -1);

   for (int i = 0; i < 2; i++) {
        player[i].addObject(Object(potion));
        player[i].getObject(0).setObjectDescription("回愎20點生命");
        player[i].addObject(Object(superPotion));
        player[i].getObject(1).setObjectDescription("回愎60點生命");
        player[i].addObject(Object(hyperPotion));
        player[i].getObject(2).setObjectDescription("回愎120點生命");
        player[i].addObject(Object(maxPotion));
        player[i].getObject(3).setObjectDescription("回愎全部生命");
   }

}

void Game::nextRound()
{
    log.clear();

    // B&P
    EffectManager::useEffect(&player[humanIndex].getCurrentCreature(), turn);
    EffectManager::useEffect(&player[computerIndex].getCurrentCreature(), turn);

    // check death
    auto &playerCreature = player[humanIndex].getCurrentCreature();
    auto &enemyCreature = player[computerIndex].getCurrentCreature();

    ++turn;
}

void Game::setTesting()
{
    isTesting = true;
}

std::string Game::useObject(Object* object, Creature* goal)
{


    cout << "[Turn " << turn << "] " << "You used a " << object->getName() << "!" << endl;
    cout << "[Turn " << turn << "] " << goal->getName() << " had its HP restored." << endl;
    return object->effectToCreature(goal);
}

void Game::useSkill(int skillIndex, Creature& goal)
{
    srand(time(0));

    stringstream result;

    auto currentSkill = player[currentPlayerIndex].getCurrentCreature().getSkill(skillIndex);

    //cout << "[Turn "<<turn<<"] "<< player[currentPlayerIndex].getCurrentCreature().getName() << " used " << currentSkill.name << "!\n";

    player[currentPlayerIndex].getCurrentCreature().useSkill(skillIndex, goal, turn, currentPlayerIndex == humanIndex);
}

void Game::useSkill(const string& skillName, Creature& goal)
{
    stringstream result;
}

// Intent: 交換寶可夢(活to活)，只有玩家會交換
// Post: change player creature ot creatureIndex
void Game::swapCreature(int creatureIndex)
{
    cout << "[Turn " << turn << "] " << player[humanIndex].getCurrentCreature().getName() << ", switch out!" << endl;
    cout << "[Turn " << turn << "] " << "Come back!" << endl;
    changeCreature(true, creatureIndex);
}

// Intent: 交換寶可夢
// Post: change player creature ot creatureIndex
void Game::changeCreature(bool isHuman, int creatureIndex)
{
    player[currentPlayerIndex].swapCreature(creatureIndex);
    cout << "[Turn " << turn << "] " << "Go! " << player[static_cast<int>(isHuman)].getCurrentCreature().getName() << "!" << endl;
}


// Intent: human attack
// Post: None

void Game::humanAttack(int index)
{
    currentPlayerIndex = humanIndex;
    useSkill(index, player[computerIndex].getCurrentCreature());
}

void Game::computerAttack(int index)
{
    currentPlayerIndex = computerIndex;
    useSkill(index, player[humanIndex].getCurrentCreature());
}

int Game::getHumanCurrentCreatureHp() const
{
    return player[humanIndex].getCurrentCreature().getHp();
}

int Game::getComputerCurrentCreatureHp() const
{
    return player[computerIndex].getCurrentCreature().getHp();
}

const string Game::getHumanCurrentCreatureName() const
{
    return player[humanIndex].getCurrentCreature().getName();
}

const string Game::getComputerCurrentCreatureName() const
{
    return player[computerIndex].getCurrentCreature().getName();
}

void Game::printStatus()
{
    cout << "[Turn " << turn << "]" << " ";
    auto& humanCreature = player[humanIndex].getCurrentCreature();
    cout << humanCreature.getName() << " " << humanCreature.getHp();
    EffectManager::printEffect(&humanCreature);

    cout << " ";
    auto& computerCreature = player[computerIndex].getCurrentCreature();
    cout << computerCreature.getName() << " " << computerCreature.getHp();
    EffectManager::printEffect(&computerCreature);
    cout << endl;
}

void Game::printCheck()
{
    cout << "[Turn " << turn << "] ";
    auto& humanCreature = player[humanIndex].getCurrentCreature();
    for (int i = 0; i < humanCreature.getSkillSize(); i++) {
        cout << humanCreature.getSkill(i).name << " " << humanCreature.getSkill(i).PP << " ";
    }
    cout << endl;
}


Player* Game::getCurrentPlayer()
{
    return &player[currentPlayerIndex];
}

Player* Game::getNotCurrentPlayer()
{
    if (currentPlayerIndex) {
        return &player[0];
    }
    else {
        return &player[1];
    }
}



