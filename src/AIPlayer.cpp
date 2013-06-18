/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "AIPlayer.h"

AIPlayer::AIPlayer (const PlayerInfo& info) : Player (info) {
	//TODO initialize the process, wait for it to start etc
}

bool AIPlayer::isMoveReady() {
	if (!Player::isMoveReady()) {
		//TODO check if we can obtain the move from the player
	}
	
	return false;
}


void AIPlayer::play (const GamePlayer& player, const vector< Move >& moves) {
	Player::play (player, moves);
	//TODO send turn to player
}

void AIPlayer::genMove() {
	//TODO send "gen_move" to the player
}


void AIPlayer::undoTurn (const GamePlayer& player, const vector< Move >& moves) {
	Player::undoTurn (player, moves);
	//TODO send undo turn to player
}

void AIPlayer::endGame (bool win) {
	Player::endGame (win);
	//TODO kill the process
}

void AIPlayer::finishTurn() {
	Player::finishTurn();
	//TODO/FIXME add SIGSTOP option
}

void AIPlayer::startTurn() {
	Player::startTurn();
	//TODO/FIXME add SIGCONT option
}

void AIPlayer::setMove (const Move& move) {
	Player::setMove (move);
}
