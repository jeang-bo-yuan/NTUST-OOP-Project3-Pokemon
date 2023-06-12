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
    // skillLib.loadFromFile(moveLibPath);
    skillLib.loadFromFile("D:/GitHub/Pokemon/project/test case/MoveLib.txt");
    // creatureLib.loadFromFile(pokemonLibPath);
    creatureLib.loadFromFile("D:/GitHub/Pokemon/project/test case/PokemonLib.txt");
    
    // ifstream gameData(gameDataPath);
    ifstream gameData("D:/GitHub/Pokemon/project/test case/GameData.txt");

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

std::string Game::useSkill(int skillIndex, Creature* goal)
{
    srand(time(0));

    stringstream result;

    auto currentSkill = player[currentPlayerIndex].getCurrentCreature().getSkill(skillIndex);

    result<<"[Turn "<<turn<<"] "<< player[currentPlayerIndex].getCurrentCreature().getName() << " used " << currentSkill.name << "!\n";

    player[currentPlayerIndex].getCurrentCreature().useSkill(skillIndex, goal);
    return result.str();
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

