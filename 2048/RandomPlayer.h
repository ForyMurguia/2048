#pragma once
#include <stack>
#include "GamePlayer.h"

class RandomPlayer : public GamePlayer {
private:
	stack<GameState> myGameStack;

public:
	RandomPlayer();
	~RandomPlayer();

	// Inherited via GamePlayer
	virtual InputDirection getMoveDirection() override;
	virtual void makeMove(InputDirection direction, int8_t newPosition, int8_t newValue) override;
	virtual void setGameState(GameState & currentState) override;
	virtual void undoMove() override;
};

