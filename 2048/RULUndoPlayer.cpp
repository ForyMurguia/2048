#include "RULUndoPlayer.h"


RULUndoPlayer::RULUndoPlayer()
{
}


RULUndoPlayer::~RULUndoPlayer()
{
}

InputDirection RULUndoPlayer::getMoveDirection()
{
	GameState newState;
	newState.copy(myGameStack.top());
	if (newState.move(RIGHT)) {
		return RIGHT;
	}
	if (newState.move(UP)) {
		return UP;
	}
	if (newState.move(LEFT)) {
		return LEFT;
	}
	return UNDO;
}
