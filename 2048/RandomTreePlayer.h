#pragma once
#include "GamePlayer.h"
#include "RandomTreeNode.h"
#include <vector>
#include <thread>
#include <mutex>

enum ExploringState {
	IDLE, EXPLORING, FINISHING,
};

class RandomTreePlayer : public GamePlayer {
private:
	volatile static int64_t ourTopPoints;
	volatile ExploringState myExploringState;
	thread *myExploringThread = nullptr;
	mutex myExploringMutex;
	RandomTreeNode *myRoot = nullptr;
	RandomTreeNode *myCurrentNode = nullptr;

	void clear();
	void exploringFunction();

public:
	RandomTreePlayer();
	~RandomTreePlayer();

	// Inherited via GamePlayer
	virtual InputDirection getMoveDirection() override;
	virtual void makeMove(InputDirection direction, int8_t newPosition, int8_t newValue) override;
	virtual void undoMove() override;
	virtual void setGameState(GameState &currentState) override;
};

