#pragma once
#include "GameState.h"

class GamePlayer
{
public:
	void setGameState(GameState &currentState) {
		gameState.copy(currentState);
	}

	virtual InputDirection getMove() = 0;
	virtual InputDirection makeMove(InputDirection direction) = 0;

protected:
	GameState gameState;
};

