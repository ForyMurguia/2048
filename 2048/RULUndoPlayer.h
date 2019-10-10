#pragma once
#include <stack>
#include "GamePlayer.h"

class RULUndoPlayer :
	public GamePlayer
{
public:
	RULUndoPlayer();
	~RULUndoPlayer();

	// Inherited via GamePlayer
	virtual InputDirection getMoveDirection() override;
};

