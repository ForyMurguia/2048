#pragma once
#include "GameState.h"

class GamePlayer
{
public:
	virtual void setGameState(GameState &currentState) = 0;
	virtual InputDirection getMoveDirection() = 0;
	virtual void makeMove(InputDirection direction, int8_t newPosition, int8_t newValue) = 0;
	virtual void undoMove() = 0;
};

