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





