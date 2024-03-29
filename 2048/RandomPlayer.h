#pragma once
#include <stack>
#include "GamePlayer.h"

class RandomPlayer : public GamePlayer {
public:
	RandomPlayer();
	~RandomPlayer();

	// Inherited via GamePlayer
	virtual InputDirection getMoveDirection() override;
};

