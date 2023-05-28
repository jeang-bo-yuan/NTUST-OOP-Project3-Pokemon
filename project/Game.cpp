#include "Game.h"

using namespace std;

Game::Game()
{
	player1 = new Player();
	player2 = new Player();
	
	Object* potion = new Object("Potion", 20);
	Object* superPotion = new Object("Super Potion", 60);
	Object* hyperPotion = new Object("Hyper Potion", 120);
	Object* maxPotion = new Object("Max Potion", -1);

	player1->objects.push_back(potion);
	player1->objects.push_back(superPotion);
	player1->objects.push_back(hyperPotion);
	player1->objects.push_back(maxPotion);
	player2->objects.push_back(potion);
	player2->objects.push_back(superPotion);
	player2->objects.push_back(hyperPotion);
	player2->objects.push_back(maxPotion);
}

void Game::newGame()
{
	isTesting = false;
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
	stringstream result;

	


	return ;
}
