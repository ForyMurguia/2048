#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

const uint8_t BOARD_SIZE = 4;

enum InputDirection {
	UP, RIGHT, LEFT, DOWN, NONE, UNDO,
};
const string inputNames[6] = { "UP", "RIGHT", "LEFT", "DOWN", "UNDO", "NONE" };

const int64_t tilePoints[4][4] = { {1 << 12, 1 << 13, 1 << 14, 1 << 15},
							{1 << 11, 1 << 10, 1 << 9, 1 << 8},
							{1 << 4, 1 << 5, 1 << 6, 1 << 7},
							{1 << 3, 1 << 2, 1 << 1, 1} };

class GameState
{
public:
	GameState();
	~GameState();

	void startNewGame();
	string getTileString();
	string getStateString();
	void print();
	bool move(InputDirection);
	int64_t getNumSteps();
	int64_t getPoints();
	void addRandomTile();
	void copy(GameState &other);
	uint8_t getTile(int r, int c);
	int getNumFreeTiles();
	int64_t getFitness();
	void addTile(uint8_t position, uint8_t value);
	int8_t randomTile();
	int8_t randomTileValue();

private:
	uint8_t myBoard[BOARD_SIZE][BOARD_SIZE];
	uint8_t myNumFreeTiles;
	uint64_t myPoints;
	uint64_t myNumSteps;
	bool mergeTiles(InputDirection direction);
	bool shiftTiles(InputDirection direction);
	void initialize();
	int getStepRowR(InputDirection direction);
	int getStepRowC(InputDirection direction);
	int getStepColumnR(InputDirection direction);
	int getStepColumnC(InputDirection direction);
	int getStartR(InputDirection direction);
	int getStartC(InputDirection direction);
};

