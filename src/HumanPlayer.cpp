/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "HumanPlayer.h"

HumanPlayer::HumanPlayer() : Player::Player() {
	this->ready = true;
}

void HumanPlayer::endGame (bool win) {
	//TODO, possibly nothing
}

void HumanPlayer::setMove (const Move& move) {
	this->moveReady = true;
	this->move = move;
}


