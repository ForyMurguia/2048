#include "RandomTreePlayer.h"

void RandomTreePlayer::clear() {
	if (myExploringThread) {
		myExploringState = FINISHING;
		myExploringThread->join();
		delete myExploringThread;
		myExploringThread = nullptr;
	}
	if (myRoot) {
		delete myRoot;
		myRoot = nullptr;
	}
}

void RandomTreePlayer::exploringFunction() {
	cout << "Starting thread with state:" << endl << myGameStack.top().getStateString() << endl;
	while (myExploringState != FINISHING) {
		myExploringMutex.lock();
    	myCurrentNode->explorePlayer(myGameStack.top());
		myExploringMutex.unlock();
		this_thread::sleep_for(chrono::microseconds(100));
	}
}

RandomTreePlayer::RandomTreePlayer()
{
}

RandomTreePlayer::~RandomTreePlayer()
{
	clear();
}

InputDirection RandomTreePlayer::getMoveDirection()
{
	int8_t bestOption = UNDO;
	double fit = -1000.0;
	myExploringMutex.lock();
	for (int i = 0; i < 100; i++) {
		myCurrentNode->explorePlayer(myGameStack.top());
	}
	for (int i = 0; i < 4; i++) {
		auto currChild = myCurrentNode->getChild(i);
		if (currChild) {
			if (currChild->getFitness() > fit) {
				bestOption = i;
				fit = currChild->getFitness();
			}
		}
	}
	myExploringMutex.unlock();
	cout << "Estimated fitness: " << fit << " for " << inputNames[bestOption] << endl;
	return static_cast<InputDirection>(bestOption);
}

void RandomTreePlayer::makeMove(InputDirection direction, int8_t newPosition, int8_t newValue)
{
	myExploringMutex.lock();
	if (myCurrentNode) {
		myCurrentNode = myCurrentNode->makeMove(direction, newPosition, newValue, myGameStack.top());
	}
	GamePlayer::makeMove(direction, newPosition, newValue);
	myExploringMutex.unlock();
}

void RandomTreePlayer::undoMove() {
	myExploringMutex.lock();
	GamePlayer::undoMove();
	if (myCurrentNode) {
		myCurrentNode = myCurrentNode->getParent()->getParent();
	}
	myExploringMutex.unlock();
}

void RandomTreePlayer::setGameState(GameState & currentState) {
	clear();
	GamePlayer::setGameState(currentState);

	myExploringState = EXPLORING;
	myRoot = new RandomTreeNode();
	myRoot->initPlayerNode(currentState, nullptr);
	myCurrentNode = myRoot;
	myExploringThread = new thread(&RandomTreePlayer::exploringFunction, this);
}
