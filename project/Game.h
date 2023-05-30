#pragma once
#include "Player.h"
#include <map>
#include <vector>
#include <sstream>

std::map<std::string, int> effectIndex =
{
	{"normal",0},
	{"fire",1},
	{"water",2},
	{"electric",3},
	{"grass",4},
	{"ice",5},
	{"fighting",6},
	{"poison",7},
	{"ground",8},
	{"flying",9},
	{"psychic",10},
	{"bug",11},
	{"rock",12},
	{"ghost",13},
	{"dragon",14},
	{"dark",15},
	{"steel",16},
	{"fairy",17}
};

std::vector<std::vector<float>> effectTable =
{
	{1,1,1,1,1,1,1,1,1,1,1,1,0.5,0,1,1,0.5,1},
	{1,0.5,0.5,1,2,2,1,1,1,1,1,2,0.5,1,0.5,1,2,1},
	{1,2,0.5,1,0.5,1,1,1,2,1,1,1,2,1,0.5,1,1,1},
	{1,1,2,0.5,0.5,1,1,1,0,2,1,1,1,1,0.5,1,1,1},
	{1,0.5,2,1,0.5,1,1,0.5,2,0.5,1,0.5,2,1,0.5,1,0.5,1},
	{1,0.5,0.5,1,2,0.5,1,1,2,2,1,1,1,1,2,1,0.5,1},
	{2,1,1,1,1,2,1,0.5,1,0.5,0.5,0.5,2,0,1,2,2,0.5},
	{1,1,1,1,2,1,1,0.5,0.5,1,1,1,0.5,0.5,1,1,0,2},
	{1,2,1,2,0.5,1,1,2,1,0,1,0.5,2,1,1,1,2,1},
	{1,1,1,0.5,2,1,2,1,1,1,1,2,0.5,1,1,1,0.5,1},
	{1,1,1,1,1,1,2,2,1,1,0.5,1,1,1,1,0,0.5,1},
	{1,0.5,1,1,2,1,0.5,0.5,1,0.5,2,1,1,0.5,1,2,0.5,0.5},
	{1,2,1,1,1,2,0.5,1,0.5,2,1,2,1,1,1,1,0.5,1},
	{0,1,1,1,1,1,1,1,1,1,2,1,1,2,1,0.5,1,1},
	{1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,0.5,1,0},
	{1,1,1,1,1,1,0.5,1,1,1,2,1,1,2,1,0.5,0.5,0.5},
	{1,0.5,0.5,0.5,1,2,1,1,1,1,1,1,2,1,1,1,0.5,2}
};

typedef struct skillInfo
{
	bool isAccuracy;
	short effectiveType;
	bool isCritical;

}skillInfo;

class Game
{
	Player* player1;
	Player* player2;
	int turn;
	bool isTesting = false;
	std::stringstream log;
public:
	Game();

	void newGame();

	std::string nextRound();

	void loadGame();

	void setTesting();

	std::string useObject(Object* object, class Creature* goal);

	std::string useSkill(int skillIndex, class Creature* goal);

	int getTurn();

public:
	Player* currentPlayer;
	Player* opponentPlayer;
};
