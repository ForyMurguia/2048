#include "RandomPlayer.h"

RandomPlayer::RandomPlayer()
{
}

RandomPlayer::~RandomPlayer()
{
}

InputDirection RandomPlayer::getMoveDirection()
{
	return static_cast<InputDirection>(rand()%4);
}

void RandomPlayer::makeMove(InputDirection direction, int8_t newPosition, int8_t newValue)
{
	GameState newState;
	newState.copy(myGameStack.top());
	if (newState.move(direction)) {
		newState.addTile(newPosition, newValue);
		myGameStack.push(newState);
	}
}

void RandomPlayer::setGameState(GameState & currentState)
{
	while (!myGameStack.empty()) {
		myGameStack.pop();
	}
	myGameStack.push(currentState);
}

void RandomPlayer::undoMove()
{
	if (!myGameStack.empty()) {
		myGameStack.pop();
	}
}
