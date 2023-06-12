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
    int playerCreatureNum;

    gameData >> playerCreatureNum;
    for (int i = 0; i < playerCreatureNum; i++)
    {
		string creatureName;
		gameData >> creatureName;
		player1.creatures.push_back(new Creature(creatureLib.getCreature(creatureName)));

        int skillNum;
        gameData >> skillNum;

        for (int j = 0; j < skillNum; j++)
        {
			string skillName;
			gameData >> skillName;
			player1.creatures[i]->addSkill(skillLib.getSkill(skillName));
		}
	}

    int oppentNum;
    gameData >> oppentNum;
    for (int i = 0; i < oppentNum; i++)
    {
        string creatureName;
        gameData >> creatureName;
        player2.creatures.push_back(new Creature(creatureLib.getCreature(creatureName)));

        int skillNum;
        gameData >> skillNum;

        for (int j = 0; j < skillNum; j++)
        {
			string skillName;
			gameData >> skillName;
			player2.creatures[i]->addSkill(skillLib.getSkill(skillName));
		}

    }
    

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

    Creature* source = currentPlayer->currentCreature;
    auto currentSkill = source->getSkill(skillIndex);

    result<<"[Turn "<<turn<<"] "<<source->getName()<<" used "<<currentSkill.name<<"!\n";

    source->useSkill(skillIndex, goal);
    return result.str();
}

