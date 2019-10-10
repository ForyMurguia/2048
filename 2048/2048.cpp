// 2048.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <time.h> 
#include <iostream>
#include <stack>
#include "GameState.h"
#include "GamePlayer.h"
#include "RandomPlayer.h"

using namespace std;

volatile bool playing;

/*
int64_t explore(GameState &gameState) {
	static int64_t bestSoFar = 0;
	while (true) {
		// Add a new tile
		gameState.addRandomTile();
		// make a move
		int8_t toMove = -1;
		int8_t options = 0;
		GameState moveState;
		int64_t sum = 0;
		int64_t fit[4];
		for (int i = 0; i < 4; i++) {
			moveState.copy(gameState);
			fit[i] = 0;
			if (moveState.move(static_cast<InputDirection>(i))) {
				fit[i] = fitness(gameState);
				sum += fit[i];
			}
		}
		if (sum == 0) {
			break;
		}
		int64_t rnd;
		uniform_int_distribution<uint64_t> dis(0, sum - 1);
		rnd = 0;
		for (int i = 0; i < 4; i++) {
			if (rnd < fit[i]) {
				toMove = i;
				rnd = fit[i];
			}
		}
		gameState.move(static_cast<InputDirection>(toMove));
	}
	int64_t result = fitness(gameState);
	if (result > bestSoFar) {
		bestSoFar = result;
		coutMutex.lock();
		cout << ">>>>>>> Improved best: <<<<<<<<" << endl;
		cout << gameState.getTileString() << "Points: " << gameState.getPoints() << ", in " << gameState.getNumSteps() << " steps, fitness: " << result << endl;
		coutMutex.unlock();
	}
	return result;
}

void exploreThread() {
	GameState currentState;
	int64_t sum[4];
	int64_t bestPoints;
	int64_t numMoves;
	int8_t bestOption;
	int8_t undo = 0;
	while (playing) {
		newStateMutex.lock();
		if (newStateForThread) {
			currentState.copy(*newStateForThread);
			newStateForThread = NULL;
			for (int i = 0; i < 4; i++) {
				sum[i] = 0;
			}
			bestPoints = 0;
			bestOption = -1;
			previousOption = -1;
			numMoves = 0;
			coutMutex.lock();
			cout << "Cleaned state" << endl;
			coutMutex.unlock();
			if (undo > 0) {
				undo--;
			}
		}
		newStateMutex.unlock();
		GameState moveState;
		if (undo == 0) {
			bool valid = false;
			for (int i = 0; i < 4; i++) {
				moveState.copy(currentState);
				if (moveState.move(static_cast<InputDirection>(i))) {
					valid = true;
					sum[i] += explore(moveState);
					if (sum[i] > bestPoints) {
						bestPoints = sum[i];
						bestOption = i;
					}
				}
			}
			if (!valid) {
				bestOption = -1;
				previousOption = 4;
				undo = 20;
				continue;
			}
			else {
				numMoves++;
			}
		}
		else {
			previousOption = 4;
			bestOption = 4;
		}
		if (((numMoves % 1000) == 0) && (previousOption != bestOption) && (numMoves > 0)) {
			previousOption = bestOption;
			coutMutex.lock();
			cout << "Best option is " << inputNames[bestOption] << " with " << bestPoints / numMoves << " points in " << numMoves << " moves" << endl;
			coutMutex.unlock();
		}
	}
}
*/

int main() {
    cout << "Hello 2048!\n"; 
	srand((int)time(NULL));
	GameState gameState, newState;
	gameState.startNewGame();
	stack<GameState> gameStack;
	playing = true;
	cout << gameState.getStateString() << endl;
	GamePlayer *player = new RandomPlayer;
	while (playing) {
		string input;
		cin >> input;
		for (int i = 0; i < input.length(); i++) {
			cout << "Processing \"" << input[i] << "\" ("  << i + 1 << "/" << input.length() << ")" << endl;
			bool moved = false;
			InputDirection trying = NONE;
			switch (toupper(input[i])) {
			case 'W':
				trying = UP;
				break;
			case 'S':
				trying = DOWN;
				break;
			case 'A':
				trying = LEFT;
				break;
			case 'D':
				trying = RIGHT;
				break;
			case 'R':
				trying = static_cast<InputDirection>(rand() % 4);
				break;
			case 'E':
				trying = static_cast<InputDirection>(rand() % 4);
				break;
			case 'U':
				trying = UNDO;
				break;
			case 'Q':
				playing = false;
				break;
			case 'P':
				cout << gameState.getStateString() << endl;
				break;
			default:
				cout << "Invalid input: " << input[i] << endl;
				continue;
			}
			if (trying != NONE) {
				cout << "Trying with: " << inputNames[trying] << endl;
				if (trying == UNDO) {
					if (!gameStack.empty()) {
						gameState.copy(gameStack.top());
						gameStack.pop();
						cout << gameState.getStateString() << endl;
					}
					else {
						cout << "Cannot undo" << endl;
					}
				}
				else {
					newState.copy(gameState);
					moved = newState.move(trying);
					if (moved) {
						gameStack.push(gameState);
						gameState.copy(newState);
						gameState.addRandomTile();
						cout << gameState.getStateString() << endl;
					}
					else {
						cout << "Invalid move" << endl;
					}
				}
			}
		}
	}
}
