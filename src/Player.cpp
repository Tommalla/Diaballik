/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "Player.h"

Player::Player() {
	moveReady = ready = false;
}

bool Player::isMoveReady() {
	return moveReady;
}

bool Player::isReady() {
	return ready;
}

Move& Player::getMove() {
	return move;
}




