#pragma once
#include "GameState.h"
#include <stack>

class GamePlayer
{
protected:
	stack<GameState> myGameStack;

public:
	virtual InputDirection getMoveDirection() = 0;

	virtual void makeMove(InputDirection direction, int8_t newPosition, int8_t newValue) {
		GameState newState;
		newState.copy(myGameStack.top());
		if (newState.move(direction)) {
			newState.addTile(newPosition, newValue);
			myGameStack.push(newState);
		}
	}

	virtual void undoMove() {
		if (!myGameStack.empty()) {
			myGameStack.pop();
		}
	}

	virtual void setGameState(GameState &currentState) {
		while (!myGameStack.empty()) {
			myGameStack.pop();
		}
		myGameStack.push(currentState);
	}
};

