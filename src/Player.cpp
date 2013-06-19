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

//Those need no implementation in this superclass, but I'm leaving
//them here so if there were any derived classes that would not need to 
//implement anything specific for those methods, they wouldn't have to
void Player::play (const GamePlayer& player, const vector< Move >& moves) {}
void Player::undoTurn (const GamePlayer& player, const vector< Move >& moves) {}

void Player::endGame (bool win) {
	this->ready = false;
	this->moveReady = false;
}

void Player::newGame (const vector< Point > black, const vector< Point > white, const vector< Point > balls, const GamePlayer& player) {

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
	this->move = move;
	qDebug("Player received a move! %d %d -> %d %d", move.from.x, move.from.y, move.to.x, move.to.y);
}

