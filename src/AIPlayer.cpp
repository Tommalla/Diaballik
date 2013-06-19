/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "AIPlayer.h"
#include "SettingsHandler.h"
#include "../DiaballikEngine/src/functions.h"
#include "gameConstants.h"
#include "StateHandler.h"

void AIPlayer::emptyQueue() {
	while (this->movesQueue.empty() == false)
		this->movesQueue.pop();
}


AIPlayer::AIPlayer (const PlayerInfo& info) : Player (info) {
	this->processing = true;
	
	qDebug("Starting bot %s", qPrintable(info.botPath));
	this->bot.setStandardErrorFile(info.name + ".out");
	this->bot.start(info.botPath);
	this->bot.waitForStarted();
	
	this->ready = true;
	this->processing = false;
}

bool AIPlayer::isMoveReady() {
	if (this->bot.isOpen() == false) {
		emit crashed();
		return false;
	}
	
	if (Player::isMoveReady() && (this->moveTimer.elapsed() >= 
		SettingsHandler::getInstance().value("bots/movesDelay", DEFAULT_MOVES_DELAY).toInt() ||
		StateHandler::getInstance().isGamePaused())) {
		return true;
	} /*else qDebug("Not ready! %d, %d", this->moveTimer.elapsed(),
		SettingsHandler::getInstance().value("bots/movesDelay", DEFAULT_MOVES_DELAY).toInt());*/
	
// 	if (StateHandler::getInstance().isGamePaused())
// 		return false;
	
	if (this->processing) {
		if (this->bot.canReadLine()) {
			QString data = this->bot.readLine();
			qDebug("Read the line! %s", qPrintable(data));
			
			if (StateHandler::getInstance().isGamePaused())	//ignore if it's pause
				return false;
			
			vector<string> response = engine::splitString(data.toStdString());
			
			if (response.size() != 2)
				return false;
			
			if (response[0] != "=")
				return false;
			
			vector<Move> moves = engine::convertToMoves(response[1]);
			for (Move move: moves)
				this->movesQueue.push(move);
			
			this->processing = false;
		}
	} else {
		if (StateHandler::getInstance().isGamePaused())
				return false;
		
		if (!this->movesQueue.empty()) {
			if (this->moveReady == false) {
				this->setMove(this->movesQueue.front());
				this->moveReady = true;
			}
		} else if (!StateHandler::getInstance().isGamePaused())
			this->genMove();
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
	this->emptyQueue();
	
	assert(moves.empty() == false);
	
	Player::play (player, moves);
// 	assert(this->processing == false);
	QString cmd = QString("play ") + engine::getIdFor(player).c_str() + " ";
	
	for (Move move: moves) {
		pair<string, string> tmp = engine::convertFromMove(move);
		cmd += QString::fromStdString(tmp.first + tmp.second);
	}
	
	cmd += "\n";
	
	qDebug("Sending: %s", qPrintable(cmd));
	this->bot.write(qPrintable(cmd));
	this->bot.waitForBytesWritten();
}

void AIPlayer::genMove() {
	this->emptyQueue();
	
	this->processing = true;
	qDebug("Asking %s for move (%s)", qPrintable(this->info.name), engine::getIdFor(this->info.player).c_str());
	QString cmd = QString("gen_move ") + engine::getIdFor(this->info.player).c_str() + "\n";
	
	this->bot.write(qPrintable(cmd));
	this->bot.waitForBytesWritten();
	
	qDebug("%s", qPrintable(cmd));
	this->moveTimer.restart();
}

void AIPlayer::undoTurn (const GamePlayer& player, const vector< Move >& moves) {
	this->emptyQueue();
	
	Player::undoTurn (player, moves);
	QString cmd = QString("undo_turn ") + engine::getIdFor(player).c_str() + " ";
	
	for (Move move: moves) {
		pair<string, string> tmp = engine::convertFromMove(move);
		cmd += QString::fromStdString(tmp.first + tmp.second);
	}
	
	cmd += "\n";
	qDebug("Sending: %s", qPrintable(cmd));
	this->bot.write(qPrintable(cmd));
	this->bot.waitForBytesWritten();
}

void AIPlayer::endGame (bool win) {
	this->emptyQueue();
	Player::endGame (win);
	this->bot.kill();	//DIE!!!! ;D
}

void AIPlayer::finishTurn() {
	Player::finishTurn();
	//TODO/FIXME add SIGSTOP option
}

void AIPlayer::startTurn() {
	this->emptyQueue();
	Player::startTurn();
	//TODO/FIXME add SIGCONT option
}

void AIPlayer::setMove (const Move& move) {
	Player::setMove (move);
}
