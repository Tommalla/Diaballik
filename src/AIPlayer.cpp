/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "AIPlayer.h"
#include "SettingsHandler.h"
#include "../DiaballikEngine/src/functions.h"
#include "gameConstants.h"

AIPlayer::AIPlayer (const PlayerInfo& info) : Player (info) {
	this->processing = true;
	
	qDebug("Starting bot %s", qPrintable(info.botPath));
	this->bot.start(info.botPath);
	this->bot.waitForStarted();
	
	this->ready = true;
	this->processing = false;
}

bool AIPlayer::isMoveReady() {
	if (Player::isMoveReady() && this->moveTimer.elapsed() >= 
		SettingsHandler::getInstance().value("bots/movesDelay", DEFAULT_MOVES_DELAY).toInt()) {
		return true;
	} else qDebug("Not ready! %d, %d", this->moveTimer.elapsed(),
		SettingsHandler::getInstance().value("bots/movesDelay", DEFAULT_MOVES_DELAY).toInt());
	
	if (this->processing) {
		if (this->bot.canReadLine()) {
			QString data = this->bot.readLine();
			qDebug("Read the line! %s", qPrintable(data));
			
			
			vector<string> response = engine::splitString(data.toStdString());
			
			if (response.size() != 2)
				return false;
			
			if (response[0] != "=")
				return false;
			
			vector<Move> moves = engine::convertToMoves(response[1]);
			for (Move move: moves) {
				qDebug("Adding %d %d -> %d %d", move.from.x, move.from.y, move.to.x, move.to.y);
				this->movesQueue.push(move);
			}
			
			if (bot.canReadLine()) {
				data = this->bot.readLine();
				qDebug("post-reading: read: %s", qPrintable(data));
			}
			this->processing = false;
		}
	} else {
		if (!this->movesQueue.empty()) {
			if (this->moveReady == false) {
				this->setMove(this->movesQueue.front());
				this->moveReady = true;
			}
		} else this->genMove();
	}
	
	return false;
}

const Move AIPlayer::getMove() {
	Move res = Player::getMove();
	this->movesQueue.pop();
	if (this->movesQueue.empty())	//no more moves in the turn
		this->finishTurn();
	
	this->moveTimer.restart();
	return res;
}


void AIPlayer::play (const GamePlayer& player, const vector< Move >& moves) {
	Player::play (player, moves);
	assert(this->processing == false);
	QString cmd = QString("play ") + engine::getIdFor(player).c_str() + " ";
	
	for (Move move: moves) {
		pair<string, string> tmp = engine::convertFromMove(move);
		cmd += QString::fromStdString(tmp.first + tmp.second);
	}
	
	cmd += "\n";
	
	qDebug("Sending: %s", qPrintable(cmd));
	this->bot.write(qPrintable(cmd));
	this->bot.waitForBytesWritten();
	
	if (bot.canReadLine()) {
		cmd = bot.readLine();
		qDebug("%s", qPrintable(cmd));
		if (cmd[0] == '=')
			bot.readLine();
	}
}

void AIPlayer::genMove() {
	this->processing = true;
	qDebug("Asking %s for move (%s)", qPrintable(this->info.name), engine::getIdFor(this->info.player).c_str());
	QString cmd = QString("gen_move ") + engine::getIdFor(this->info.player).c_str() + "\n";
	this->bot.write(qPrintable(cmd));
	this->bot.waitForBytesWritten();
	qDebug("%s", qPrintable(cmd));
	this->moveTimer.restart();
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
