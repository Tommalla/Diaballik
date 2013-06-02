/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include "Player.h"

Player::Player() {
	this->moveReady = this->ready = false;
	this->turnFinished = true;
}

bool Player::isMoveReady() {
	return this->isReady() && this->moveReady;
}

bool Player::isReady() {
	return this->ready;
}

bool Player::isTurnFinished() {
	return this->turnFinished;
}

const Move Player::getMove() {
	assert(this->isMoveReady());
	Move moveCopy = this->move;
	this->move = Move(Point(-1, -1), Point(-1, -1));
	return moveCopy;
}

void Player::finishTurn() {
	this->turnFinished = true;
}




