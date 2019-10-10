#pragma once
#include "GameState.h"

class GamePlayer
{
protected:
	stack<GameState> myGameStack;

public:
	virtual InputDirection getMoveDirection() = 0;

	void makeMove(InputDirection direction, int8_t newPosition, int8_t newValue) {
		GameState newState;
		newState.copy(myGameStack.top());
		if (newState.move(direction)) {
			newState.addTile(newPosition, newValue);
			myGameStack.push(newState);
		}
	}

	void undoMove() {
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

