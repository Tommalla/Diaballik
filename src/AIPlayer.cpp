/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "AIPlayer.h"
#include "../DiaballikEngine/src/functions.h"

AIPlayer::AIPlayer (const PlayerInfo& info) : Player (info) {
	this->processing = true;
	
	qDebug("Starting bot %s", qPrintable(info.botPath));
	this->bot.start(info.botPath);
	this->bot.waitForStarted();
	
	this->processing = false;
}

bool AIPlayer::isMoveReady() {
	if (Player::isMoveReady()) 
		return true;
	
	if (this->processing) {
		if (this->bot.canReadLine()) {
			QString data = this->bot.readLine();
			qDebug("Read the line! %s", qPrintable(data));
			
			vector<string> response = engine::splitString(data.toStdString());
			
			assert(response.size() == 2);
			assert(response[0] == "="); 
			
			if (response[0] != "=")
				return false;
			
			vector<Move> moves = engine::convertToMoves(response[1]);
			for (Move move: moves) {
				qDebug("Adding %d %d -> %d %d", move.from.x, move.from.y, move.to.x, move.to.y);
				this->movesQueue.push(move);
			}
			this->processing = false;
		}
	} else {
		if (!this->movesQueue.empty()) {
			this->setMove(this->movesQueue.front());
			this->movesQueue.pop();
			this->moveReady = true;
			return true;
		} else this->genMove();
	}
	
	return false;
}

const Move AIPlayer::getMove() {
	Move res = Player::getMove();
	if (this->movesQueue.empty())	//no more moves in the turn
		this->finishTurn();
	return res;
}


void AIPlayer::play (const GamePlayer& player, const vector< Move >& moves) {
	Player::play (player, moves);
	//TODO send turn to player
}

void AIPlayer::genMove() {
	this->processing = true;
	QString cmd = QString("gen_move ") + engine::getIdFor(this->info.player).c_str() + "\n";
	this->bot.write(qPrintable(cmd));
}

void AIPlayer::undoTurn (const GamePlayer& player, const vector< Move >& moves) {
	Player::undoTurn (player, moves);
	//TODO send undo turn to player
}

void AIPlayer::endGame (bool win) {
	Player::endGame (win);
	this->bot.kill();	//DIE!!!! ;D
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
