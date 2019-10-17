#include "RandomTreeNode.h"

RandomTreeNode::RandomTreeNode()
{
}

RandomTreeNode::~RandomTreeNode()
{
	for (auto child : myChildren) {
		if (child)
			delete child;
	}
}

double RandomTreeNode::getFitness() {
	return myFitness;
}

void RandomTreeNode::initPlayerNode(GameState & gameState, RandomTreeNode *parent) {
	myParent = parent;
	myFitness = double(gameState.getFitness());
	myChildren.resize(4);
	for (int i = 0; i < 4; i++) {
		myChildren[i] = nullptr;
	}
}

void RandomTreeNode::initRandomNode(GameState & gameState, RandomTreeNode *parent) {
	myParent = parent;
	myChildren.resize(gameState.getNumFreeTiles() * 2);
	for (int i = gameState.getNumFreeTiles() - 1; i >= 0; i--) {
		GameState newGameState;
		newGameState.copy(gameState);
		newGameState.addTile(i, 1);
		myChildren[i * 2] = new RandomTreeNode;
		myChildren[i * 2]->initPlayerNode(newGameState, this);
		newGameState.copy(gameState);
		newGameState.addTile(i, 2);
		myChildren[i * 2 + 1] = new RandomTreeNode;
		myChildren[i * 2 + 1]->initPlayerNode(newGameState, this);
	}
	updateRandomFitness();
}

void RandomTreeNode::updatePlayerFitness()
{
	bool empty = true;
	for (auto child : myChildren) {
		if (child) {
			double currFit = child->getFitness();
			if (empty) {
				empty = false;
				myFitness = currFit;
			}
			else {
				if (myFitness < currFit) {
					myFitness = currFit;
				}
			}
		}
	}
}

void RandomTreeNode::updateRandomFitness() {
	int8_t num_child = 0;
	for (auto child : myChildren) {
		if (child) {
			double currFit = child->getFitness();
			if (num_child == 0) {
				myFitness = currFit;
			}
			else {
				myFitness += currFit;
			}
			num_child++;
		}
	}
	if (num_child > 0) {
		myFitness /= num_child;
	}
}

RandomTreeNode* RandomTreeNode::getParent() {
	return myParent;
}

RandomTreeNode * RandomTreeNode::getChild(int8_t index) {
	return myChildren[index];
}

RandomTreeNode * RandomTreeNode::addChild(int8_t index, GameState & gameState)
{
	myChildren[index] = new RandomTreeNode;
	myChildren[index]->initRandomNode(gameState, this);
	return myChildren[index];
}

RandomTreeNode * RandomTreeNode::makeMove(InputDirection direction, int8_t newPosition, int8_t newValue, GameState &gameState) {
	if (myChildren[direction] == nullptr) {
		myChildren[direction] = new RandomTreeNode;
		GameState newGameState;
		newGameState.copy(gameState);
		newGameState.move(direction);
		myChildren[direction]->initRandomNode(newGameState, this);
	}
	for (int i = 1; i < 4; i++) {
		auto currNode = myChildren[(direction + i) % 4];
		if (currNode) {
			delete currNode;
			myChildren[(direction + i) % 4] = nullptr;
		}
	}

	return myChildren[direction]->getChild(newPosition * 2 + (newValue - 1));
}

void RandomTreeNode::explorePlayer(GameState &gameState) {
	for (int i = 0; i < 4; i++) {
		GameState newGameState;
		newGameState.copy(gameState);
		if (newGameState.move(static_cast<InputDirection>(i))) {
			if (myChildren[i] == nullptr) {
				myChildren[i] = new RandomTreeNode;
				myChildren[i]->initRandomNode(newGameState, this);
			}
			myChildren[i]->exploreRandom(newGameState);
		}
	}
}

void RandomTreeNode::exploreRandom(GameState & gameState) {
	static 
	GameState currentState;
	GameState newState;
	currentState.copy(gameState);
	RandomTreeNode *currentNode = this;
	while (true) {
		int8_t newPosition = currentState.randomTile();
		int8_t newValue = currentState.randomTileValue();
		currentState.addTile(newPosition, newValue);
		int64_t currPoints = currentState.getPoints();
		if (currPoints > ourTopPoints) {
			ourTopPoints = currPoints;
			cout << "Improved to " << currPoints << endl;
		}
		currentNode = currentNode->getChild(newPosition * 2 + newValue - 1);
		int8_t numValid = 0;
		int8_t validDir[4];
		for (int i = 0; i < 4; i++) {
			newState.copy(currentState);
			if (newState.move(static_cast<InputDirection>(i))) {
				validDir[numValid] = i;
				numValid++;
			}
		}
		if (numValid == 0) {
			break;
		}
		int8_t newDirection = validDir[rand() % numValid];
		currentState.move(static_cast<InputDirection>(newDirection));
		if (currentNode->getChild(newDirection) == nullptr) {
			currentNode = currentNode->addChild(newDirection, currentState);
		}
		else {
			currentNode = currentNode->getChild(newDirection);
		}
	}

	

	while (currentNode != myParent) {
		currentNode->updatePlayerFitness();
		currentNode = currentNode->getParent();
		currentNode->updateRandomFitness();
		currentNode = currentNode->getParent();
	}
}

volatile int64_t RandomTreeNode::ourTopPoints = 0;