/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "AIPlayer.h"
#include "SettingsHandler.h"
#include "../DiaballikEngine/src/functions.h"
#include "gameConstants.h"
#include "StateHandler.h"
#include <QDateTime>

void AIPlayer::emptyQueue() {
	this->movesQueue.clear();
}


AIPlayer::AIPlayer (const PlayerInfo& info) : Player (info) {
	lastTurnUndone = NONE;
	this->processing = true;
	
	qDebug("Starting bot %s", qPrintable(info.botPath));
	this->bot.setStandardErrorFile(BOTS_DIR + "logs/"+ 
		QDateTime::currentDateTime().toString("d-M-yyyy:hh:mm") + this->info.name + ".out");
	this->bot.start(info.botPath);
	this->bot.waitForStarted();
	
	this->ready = true;
	this->processing = false;
}

bool AIPlayer::isMoveReady() {
	if (this->bot.state() == QProcess::NotRunning) {
		emit crashed();
		return false;
	}

	
	if (Player::isMoveReady() && (this->moveTimer.elapsed() >= 
		SettingsHandler::getInstance().value("bots/movesDelay", DEFAULT_MOVES_DELAY).toInt() ||
		StateHandler::getInstance().isGamePaused())) {
		return true;
	}
	
	if (this->processing) {
		if (this->bot.canReadLine()) {
			QString data = this->bot.readLine();
			qDebug("Read the line! %s", qPrintable(data));
			
			vector<string> response = engine::splitString(data.toStdString());
			
			if (response.size() != 2)
				return false;
			
			if (response[0] != "=")
				return false;

			this->processing = false;
			
			if (StateHandler::getInstance().isGamePaused())	//ignore if it's pause
				return false;
			
			vector<Move> moves = engine::convertToMoves(response[1]);
			for (Move move: moves)
				this->movesQueue.enqueue(move);
		}
	} else {
		if (StateHandler::getInstance().isGamePaused())
				return false;
		
		if ((int)this->movesQueue.size() > 0) {
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
	
	if (!this->movesQueue.empty()) {
		this->movesQueue.dequeue();
		if (this->movesQueue.empty())	//no more moves in the turn
			this->finishTurn();
	}
	
	this->moveTimer.restart();
	return res;
}


void AIPlayer::play (const GamePlayer& player, const vector< Move >& moves) {
	this->movesQueue.clear();
	
	assert(moves.empty() == false);
	
	if (this->playedMoves.empty() == false && this->playedMoves.back() == moves)
		return;
	
	Player::play (player, moves);
// 	assert(this->processing == false);
	QString cmd = QString("play ") + engine::getIdFor(player).c_str() + " ";
	
	for (Move move: moves) {
		pair<string, string> tmp = engine::convertFromMove(move);
		cmd += QString::fromStdString(tmp.first + tmp.second);
	}
	
	cmd += "\n";
	
	qDebug("Sending to %s: %s", qPrintable(this->info.name), qPrintable(cmd));
	this->bot.write(qPrintable(cmd));
	this->bot.waitForBytesWritten();
	
	this->playedMoves.push_back(moves);
}

void AIPlayer::genMove() {
	this->movesQueue.clear();
	
	this->processing = true;
	QString cmd = QString("gen_move ") + engine::getIdFor(this->info.player).c_str() + "\n";
	
	this->bot.write(qPrintable(cmd));
	this->bot.waitForBytesWritten();
	
	qDebug("Sending to %s: %s", qPrintable(this->info.name), qPrintable(cmd));
	this->moveTimer.restart();
}

void AIPlayer::undoTurn (const GamePlayer& player, const vector< Move >& moves) {
	qDebug("Called undo turn on %s", qPrintable(this->info.name));
	
	if (this->playedMoves.empty() || this->playedMoves.back() != moves)
		return;
	
	this->movesQueue.clear();
	
	Player::undoTurn (player, moves);
	QString cmd = QString("undo_turn ") + engine::getIdFor(player).c_str() + " ";
	
	for (Move move: moves) {
		pair<string, string> tmp = engine::convertFromMove(move);
		cmd += QString::fromStdString(tmp.first + tmp.second);
	}
	
	cmd += "\n";
	qDebug("Sending to %s: %s", qPrintable(this->info.name), qPrintable(cmd));
	this->bot.write(qPrintable(cmd));
	this->bot.waitForBytesWritten();
	
	this->playedMoves.pop_back();
}

void AIPlayer::endGame (bool win) {
	this->emptyQueue();
	Player::endGame (win);
	this->bot.kill();	//DIE!!!! ;D
}

void AIPlayer::newGame (const vector< Point > black, const vector< Point > white, const vector< Point > balls, const GamePlayer& player) {
	Player::newGame (black, white, balls, player);
	
	this->movesQueue.clear();
	this->processing = false;
	QString cmd = "new_game ";
	
	vector<Point> figures[2] = {black, white};
	
	for (int i = 0; i < 2; ++i)
		for (Point pawn: figures[i])
			cmd += QString::fromStdString(engine::convertFromPoint(pawn));
	cmd += " ";
	for (Point ball: balls)
		cmd += QString::fromStdString(engine::convertFromPoint(ball));
	cmd += QString(" ") + QString::fromStdString(engine::getIdFor(player)) + "\n";
	
	this->bot.write(qPrintable(cmd));
	qDebug("Sending to %s: %s", qPrintable(this->info.name), qPrintable(cmd));
	this->bot.waitForBytesWritten();
}


void AIPlayer::finishTurn() {
	Player::finishTurn();
	//TODO/FIXME add SIGSTOP option (configurable via settings)
}

void AIPlayer::startTurn() {
	this->emptyQueue();
	Player::startTurn();
	//TODO/FIXME add SIGCONT option
}

void AIPlayer::setMove (const Move& move) {
	Player::setMove (move);
}
