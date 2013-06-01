/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include "Player.h"

Player::Player() {
	this->moveReady = this->ready = false;
}

bool Player::isMoveReady() {
	return this->isReady() && this->moveReady;
}

bool Player::isReady() {
	return this->ready;
}

Move& Player::getMove() {
	assert(this->isMoveReady());
	return this->move;
}




