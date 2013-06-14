/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include "Player.h"

Player::Player(const PlayerInfo& info) {
	this->info = info;
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
	this->moveReady = false;
	return moveCopy;
}

const PlayerInfo& Player::getPlayerInfo() const {
	return info;
}

void Player::undoTurn (const vector< Move > moves) {
	//empty
}


void Player::finishTurn() {
	this->turnFinished = true;
}

void Player::startTurn() {
	this->turnFinished = false;
}

void Player::setMove (const Move& move) {
	if (this->turnFinished)
		return;
	
	this->moveReady = true;
	//this->turnFinished = false;
	this->move = move;
	qDebug("Player received a move!");
}

