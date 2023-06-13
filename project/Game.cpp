#include "Game.h"
#include <random>
#include <fstream>

using namespace std;


Game::Game()
{
    newGame();
}



void Game::loadGame(std::string pokemonLibPath,std::string moveLibPath,std::string gameDataPath)
{
    newGame();
    skillLib.loadFromFile(moveLibPath);
//    skillLib.loadFromFile("D:/GitHub/Pokemon/project/test case/MoveLib.txt");
    creatureLib.loadFromFile(pokemonLibPath);
//    creatureLib.loadFromFile("D:/GitHub/Pokemon/project/test case/PokemonLib.txt");
    
    ifstream gameData(gameDataPath);
//    ifstream gameData("D:/GitHub/Pokemon/project/test case/GameData.txt");

    gameData >> player[0] >> player[1];
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

    Object potion("Potion", 20);
    Object superPotion("Super Potion", 60);
    Object hyperPotion("Hyper Potion", 120);
    Object maxPotion("Max Potion", -1);

   for (int i = 0; i < 2; i++) {
        player[i].addObject(Object(potion));
        player[i].addObject(Object(superPotion));
        player[i].addObject(Object(hyperPotion));
        player[i].addObject(Object(maxPotion));
   }

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

void Game::useSkill(int skillIndex, Creature& goal)
{
    srand(time(0));

    stringstream result;

    auto currentSkill = player[currentPlayerIndex].getCurrentCreature().getSkill(skillIndex);

    cout << "[Turn "<<turn<<"] "<< player[currentPlayerIndex].getCurrentCreature().getName() << " used " << currentSkill.name << "!\n";

    player[currentPlayerIndex].getCurrentCreature().useSkill(skillIndex, goal, turn);
}

void Game::swapCreature(int creatureIndex)
{
    currentPlayerIndex = humanIndex;
    cout << "[Turn " << turn << "] " << player[currentPlayerIndex].getCurrentCreature().getName() << ", switch out!" << endl;
    cout << "[Turn " << turn << "] " << "Come back!" << endl;
    player[currentPlayerIndex].switchCurrentCreature(creatureIndex);
    cout << "[Turn " << turn << "] " << "Go! " << player[currentPlayerIndex].getCurrentCreature().getName() << "!" << endl;

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

void Game::swapTurn()
{
    if (currentPlayerIndex) {
        currentPlayerIndex = 0;
    }
    else {
        currentPlayerIndex = 1;
    }
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



