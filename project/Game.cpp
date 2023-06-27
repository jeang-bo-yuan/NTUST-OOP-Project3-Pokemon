#include "Game.h"
#include "EffectManager.h"
#include <random>
#include <fstream>
#include <QDebug>

using namespace std;


Game::Game()
{
    newGame();
}



void Game::loadGame(std::string pokemonLibPath,std::string moveLibPath,std::string gameDataPath)
{
    newGame();

    try {
        skillLib.loadFromFile(moveLibPath);
        creatureLib.loadFromFile(pokemonLibPath);
    }
    catch (int num) {
        throw num;
    }

    ifstream gameData(gameDataPath);
    if (!gameData.is_open()) {
        cerr << "Cannot open Game Data: " << gameDataPath << endl;
        throw 1;
    }
    
    gameData >> player[0] >> player[1];

    player[0].setCreatureIsHuman(true);
    player[1].setCreatureIsHuman(false);
}

int Game::loadFromFile(const string& filename)
{
    string pokemonLibPath, moveLibPath, gameDataPath;
    string command;

    ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open command file: " << filename << std::endl;
        throw 1;
    }
    file >> pokemonLibPath >> moveLibPath >> gameDataPath;

    try {
        loadGame(pokemonLibPath, moveLibPath, gameDataPath);
    }
    catch (int num) {
        throw num;
    }

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

                humanAttack(player[humanIndex].getSkillIndex(skill1));
                if (player[computerIndex].getCurrentCreature().getHp() <= 0) goto END;
                computerAttack(player[computerIndex].getSkillIndex(skill2));
			}
            else {
                computerAttack(player[computerIndex].getSkillIndex(skill2));
                if (player[humanIndex].getCurrentCreature().getHp() <= 0) goto END;
                humanAttack(player[humanIndex].getSkillIndex(skill1));
			}
        }
        else if (command == "Bag") {
            string potion, creatureName, computerSkill;

            file >> potion >> creatureName >> computerSkill;


            useObject(&player[humanIndex].getObject(potion), &player[humanIndex].getCreature(creatureName));

            computerAttack(player[computerIndex].getSkillIndex(computerSkill));
        }
        else if (command == "Pokemon") {
            string creatureName, computerSkill;

            file >> creatureName >> computerSkill;

            int index = player[humanIndex].findCreatureIndex(creatureName);

            if (index == -1) {
                cout << "creature: " << creatureName << " not find!" << endl;
            }

            swapCreature(index);
            computerAttack(player[computerIndex].getSkillIndex(computerSkill));

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
        for (int i = 0; i < 2; i++) {
            if (isFainted(i)) {
                changeCreature(i, player[i].getCurrentCreatureIndex() + 1);
            }
        }

        if (winOrLose()) {
            return 1;
        }


        for (int i = 0; i < 2; i++) {
            if (!isFainted(i)) {
                EffectManager::useEffect(&player[i].getCurrentCreature(), turn);
            }
        }

        for (int i = 0; i < 2; i++) {
            if (isFainted(i)) {
                changeCreature(i, player[i].getCurrentCreatureIndex() + 1);
            }
        }


        if (winOrLose()) {
            return 1;
        }

        qDebug() << turn << player[humanIndex].getCurrentCreature().getName().c_str() << player[computerIndex].getCurrentCreature().getName().c_str();

        turn++;
    }

    file.close();
    return 0;
}

void Game::newGame()
{
    isTesting = false;
    player[0] = Player();
    player[1] = Player();
    currentPlayerIndex = 0;
//    pokemonLib.clear();
//    moveLib.clear();
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

void Game::nextRound_BandP()
{
    log.clear();

    // B&P
    if (!isFainted(humanIndex))
        EffectManager::useEffect(&player[humanIndex].getCurrentCreature(), turn);

    if (!isFainted(computerIndex))
        EffectManager::useEffect(&player[computerIndex].getCurrentCreature(), turn);

    // check death
//    auto &playerCreature = player[humanIndex].getCurrentCreature();
//    auto &enemyCreature = player[computerIndex].getCurrentCreature();
    isFainted(computerIndex);
    isFainted(humanIndex);
}

void Game::setTesting()
{
    isTesting = true;
}

std::string Game::useObject(Object* object, Creature* goal)
{


    cout << "[Turn " << turn << "] " << "You used a " << object->getPrintName() << "!" << endl;
    cout << "[Turn " << turn << "] " << goal->getName() << " had its HP restored." << endl;
    return object->effectToCreature(goal);
}




// Intent: 交換寶可夢(活to活)，只有玩家會交換
// Post: change player creature ot creatureIndex
void Game::swapCreature(int creatureIndex)
{
    cout << "[Turn " << turn << "] " << player[humanIndex].getCurrentCreature().getName() << ", switch out!" << endl;
    cout << "[Turn " << turn << "] " << "Come back!" << endl;
    changeCreature(false, creatureIndex);
}

// Intent: 交換寶可夢
// Post: change player creature ot creatureIndex
bool Game::changeCreature(bool isComputer, int creatureIndex)
{
    if (isComputer) {
        player[1].swapCreature(creatureIndex);
        return true;
    }
    else {
        if (!player[0].swapCreature(creatureIndex)) return false;
        cout << "[Turn " << turn << "] " << "Go! " << player[0].getCurrentCreature().getName() << "!" << endl;
        return true;
	}
  


}


// Intent: human attack
// Post: None

void Game::humanAttack(int index)
{
    auto currentSkill = player[humanIndex].getCurrentCreature().getSkill(index);

    player[humanIndex].getCurrentCreature().useSkill(index, player[computerIndex].getCurrentCreature(), turn, currentPlayerIndex == humanIndex,isTesting);
}

void Game::computerAttack(int index)
{
    auto currentSkill = player[computerIndex].getCurrentCreature().getSkill(index);

    player[computerIndex].getCurrentCreature().useSkill(index, player[humanIndex].getCurrentCreature(), turn, currentPlayerIndex == humanIndex,isTesting);
}

bool Game::isFainted(int playerIndex)
{
    auto &creature = player[playerIndex].getCurrentCreature();

    if (!creature.isFaint() && creature.getHp() == 0) {
        cout << "[Turn " << turn << "] " << creature.getPrintName() << " fainted!" << endl;
        creature.setFaint(true);
        return true;
    }
    else if (creature.isFaint() && creature.getHp() == 0) {
        return true;
    }
    else {
        return false;
    }
}

bool Game::winOrLose()
{

    for (int i = 0; i < 2; i++) {
        bool isWindOrLose = true;
        for (int j = 0; j < player[i].getCreatureSize(); j++) {
            if (!player[i].getCreature(j).isFaint()) {
				isWindOrLose = false;
				break;
			}
        }

        if (isWindOrLose) {
            if (i == 0) {
                cout << "[Turn " << turn << "] " << "You lose" << endl;
				return true;
            }
            else {
				cout << "[Turn " << turn << "] " << "You win" << endl;
            }
            return true;
        }
    }
    return false;
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



