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

int main() {
    cout << "Hello 2048!\n"; 
	srand((int)time(NULL));
	GameState gameState, newState;
	gameState.startNewGame();
	stack<GameState> gameStack;
	playing = true;
	cout << gameState.getStateString() << endl;
	GamePlayer *player = new RandomPlayer;
	player->setGameState(gameState);
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
				trying = player->getMoveDirection();
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
						player->undoMove();
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
						int8_t newTile = gameState.randomTile();
						int8_t newValue = gameState.randomTileValue();
						gameState.addTile(newTile, newValue);
						player->makeMove(trying, newTile, newValue);
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
