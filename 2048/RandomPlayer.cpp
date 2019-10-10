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
