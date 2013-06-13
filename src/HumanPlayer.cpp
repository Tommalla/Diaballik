/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(const PlayerInfo& info) : Player::Player(info) {
	this->ready = true;
}

void HumanPlayer::endGame (bool win) {
	//TODO, possibly nothing
}

void HumanPlayer::play (const vector< Move > moves) {
	qDebug("HumanPlayer::play");
	//nothing to do here
}


void HumanPlayer::setMove (const Move& move) {
	if (this->turnFinished)
		return;
	
	this->moveReady = true;
	//this->turnFinished = false;
	this->move = move;
	qDebug("HumanPlayer received a move!");
}


