#include "GameState.h"

using namespace std;

GameState::GameState()
{
	initialize();
}

GameState::~GameState()
{
}

void GameState::initialize()
{
	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			myBoard[r][c] = 0;
		}
	}
	myPoints = 0;
	myNumFreeTiles = BOARD_SIZE * BOARD_SIZE;
	myNumSteps = 0;
}

void GameState::startNewGame()
{
	initialize();
	addRandomTile();
	addRandomTile();
}

void GameState::addTile(uint8_t position, uint8_t value)
{
	if (position >= myNumFreeTiles) {
		return;
	}
	myNumFreeTiles--;
	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			if (myBoard[r][c] == 0) {
				if (position == 0) {
					myBoard[r][c] = value;
					return;
				}
				else {
					position--;
				}
			}
		}
	}
}

int8_t GameState::randomTile()
{
	if (myNumFreeTiles > 1) {
		return rand() % myNumFreeTiles;
	}
	return 0;
}

int8_t GameState::randomTileValue()
{
	return (rand() % 10) < 9 ? 1 : 2;
}

void GameState::addRandomTile()
{
	if (myNumFreeTiles == 0) {
		return;
	}
	addTile(randomTile(), randomTileValue());
}

void GameState::copy(GameState &other)
{
	for (int r = 0; r < BOARD_SIZE; r++)
		for (int c = 0; c < BOARD_SIZE; c++) {
			myBoard[r][c] = other.getTile(r, c);
	}
	myNumFreeTiles = other.getNumFreeTiles();
	myNumSteps = other.getNumSteps();
	myPoints = other.getPoints();
}

uint8_t GameState::getTile(int r, int c)
{
	return myBoard[r][c];
}

int GameState::getNumFreeTiles()
{
	return myNumFreeTiles;
}

int64_t GameState::getFitness() {
	int64_t result = getPoints();
	for (int8_t r = 0; r < 4; r++) {
		for (int8_t c = 0; c < 4; c++) {
			int8_t tile = getTile(r, c);
			if (tile > 0) {
				result += tilePoints[r][c] * (1LL << tile);
			}
		}
	}
	return result;
}

int GameState::getStepRowR(InputDirection direction)
{
	switch (direction) {
	case LEFT:
		return 1;
	case RIGHT:
		return -1;
	}
	return 0;
}

int GameState::getStepRowC(InputDirection direction)
{
	switch (direction) {
	case UP:
		return -1;
	case DOWN:
		return 1;
	}
	return 0;
}

int GameState::getStepColumnR(InputDirection direction)
{
	switch (direction) {
	case UP:
		return 1;
	case DOWN:
		return -1;
	}
	return 0;
}

int GameState::getStepColumnC(InputDirection direction)
{
	switch (direction) {
	case LEFT:
		return 1;
	case RIGHT:
		return -1;
	}
	return 0;
}

int GameState::getStartR(InputDirection direction)
{
	switch (direction) {
	case RIGHT:
	case DOWN:
		return BOARD_SIZE - 1;
	}
	return 0;
}

int GameState::getStartC(InputDirection direction)
{
	switch (direction) {
	case UP:
	case RIGHT:
		return BOARD_SIZE - 1;
	}
	return 0;
}

int64_t GameState::getNumSteps()
{
	return myNumSteps;
}

int64_t GameState::getPoints()
{
	return myPoints;
}

string GameState::getTileString()
{
	char aux[10];
	string result;
	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			result.append("+---");
		}
		result.append("+\n");
		for (int c = 0; c < BOARD_SIZE; c++) {
			char tile = myBoard[r][c];
			if (tile > 0) {
				sprintf_s(aux, 10, "|%3d", (int)tile);
				result.append(aux);
			}
			else {
				result.append("|   ");
			}
		}
		result.append("|\n");
	}
	for (int c = 0; c < BOARD_SIZE; c++) {
		result.append("+---");
	}
	return result;
}

string GameState::getStateString()
{
	return getTileString() + "\nPoints: " + to_string(getPoints()) + ", in " + to_string(getNumSteps()) + " steps, fitness: " + to_string(getFitness());
}

void GameState::print()
{
	cout << getTileString();
}

bool GameState::mergeTiles(InputDirection direction)
{
	int stepRowR = getStepRowR(direction), stepRowC = getStepRowC(direction);
	int stepColumnR = getStepColumnR(direction), stepColumnC = getStepColumnC(direction);
	int startR = getStartR(direction), startC = getStartC(direction);
	bool merged = false;
	for (int y = 0; y < BOARD_SIZE; y++) {
		int r = startR;
		int c = startC;
		for (int x = 0; x < (BOARD_SIZE - 1); x++) {
			if (myBoard[r][c] != 0) {
				// Check if it can merge with next
				int nx = x + 1;
				int nr = r + stepColumnR;
				int nc = c + stepColumnC;
				while (nx < BOARD_SIZE) {
					if (myBoard[nr][nc] != 0) {
						if (myBoard[r][c] == myBoard[nr][nc]) {
							// merge and add points
							myBoard[r][c]++;
							myBoard[nr][nc] = 0;
							myPoints += 1LL << myBoard[r][c];
							merged = true;
							myNumFreeTiles++;
							break;
						}
						else {
							// cannot merge since it is different
							break;
						}
					}
					nx++;
					nr += stepColumnR;
					nc += stepColumnC;
				}
			}
			r += stepColumnR;
			c += stepColumnC;
		}
		startR += stepRowR;
		startC += stepRowC;
	}
	return merged;
}

bool GameState::shiftTiles(InputDirection direction)
{
	int stepRowR = getStepRowR(direction), stepRowC = getStepRowC(direction);
	int stepColumnR = getStepColumnR(direction), stepColumnC = getStepColumnC(direction);
	int startR = getStartR(direction), startC = getStartC(direction);
	bool shifted = false;
	for (int y = 0; y < BOARD_SIZE; y++) {
		int r = startR;
		int c = startC;
		for (int x = 0; x < (BOARD_SIZE - 1); x++) {
			if (myBoard[r][c] == 0) {
				// Check if it can be occupied by next
				int nx = x + 1;
				int nr = r + stepColumnR;
				int nc = c + stepColumnC;
				while (nx < BOARD_SIZE) {
					if (myBoard[nr][nc] != 0) {
						myBoard[r][c] = myBoard[nr][nc];
						myBoard[nr][nc] = 0;
						shifted = true;
						break;
					}
					nx++;
					nr += stepColumnR;
					nc += stepColumnC;
				}
			}
			r += stepColumnR;
			c += stepColumnC;
		}
		startR += stepRowR;
		startC += stepRowC;
	}
	return shifted;
}

bool GameState::move(InputDirection direction)
{
	bool merged = mergeTiles(direction);
	bool shifted = shiftTiles(direction);
	bool moved = merged || shifted;
	if (moved) {
		myNumSteps++;
	}
	return moved;
}
