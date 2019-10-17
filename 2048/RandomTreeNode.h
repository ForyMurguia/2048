#pragma once
#include <stdint.h>
#include <vector>
#include "GameState.h"

using namespace std;

class RandomTreeNode
{
private:
	double myFitness;
	vector<RandomTreeNode *> myChildren;
	RandomTreeNode *myParent;
	volatile static int64_t ourTopPoints;

public:
	RandomTreeNode();
	~RandomTreeNode();

	double getFitness();
	void initPlayerNode(GameState &gameState, RandomTreeNode *parent);
	void initRandomNode(GameState &gameState, RandomTreeNode *parent);
	void updatePlayerFitness();
	void updateRandomFitness();
	RandomTreeNode* getParent();
	RandomTreeNode* getChild(int8_t index);
	RandomTreeNode* addChild(int8_t index, GameState &gameState);
	RandomTreeNode* makeMove(InputDirection direction, int8_t newPosition, int8_t newValue, GameState &gameState);
	void explorePlayer(GameState &gameState);
	void exploreRandom(GameState &gameState);
};

