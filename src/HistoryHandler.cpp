/* Tomasz [Tommalla] Zakrzewski, 2013
 * All rights reserved */

#include <cassert>
#include "HistoryHandler.h"

HistoryHandler::HistoryHandler() : QObject() {
	this->turnId = 0;
	this->lastMoveId = -1;
	this->turns.resize(1);
}


HistoryHandler::HistoryHandler (const vector< vector< Move > >& turns, const int turnId, const int lastMoveId) : QObject() {
	this->turns = turns;
	this->turnId = turnId;
	this->lastMoveId = lastMoveId;
}

const bool HistoryHandler::canUndoMove() const {
	return this->lastMoveId >= 0 || this->turnId > 0; 
}

const bool HistoryHandler::canRedoMove() const {
	return this->lastMoveId + 1 < this->turns[turnId].size() || 
		this->turnId + 1 < this->turns.size();
}

const bool HistoryHandler::canUndoTurn() const {
	return this->canUndoMove();
}

const bool HistoryHandler::canRedoTurn() const {
	return canRedoMove();
}

Move HistoryHandler::getPreviousMove() const {
	assert(this->canUndoMove());
	
	return (this->canUndoMove()) ? (
		(this->lastMoveId >= 0) ? this->turns[turnId][lastMoveId] : 
			this->turns[turnId - 1][this->turns[turnId - 1].size() - 1]) :
		Move(-1, -1, -1, -1);	//should never happen
}

Move HistoryHandler::getNextMove() const {
	assert(this->canRedoMove());
	
	return (this->canRedoMove()) ? (
		(this->lastMoveId + 1 < this->turns[turnId].size()) ? this->turns[turnId][lastMoveId + 1] :
			this->turns[turnId + 1][0]) :
		Move(-1, -1, -1, -1);
}

vector< Move > HistoryHandler::getToNextTurn() const {
	vector<Move> res;
	
	//TODO get the moves
	//but FIRST think how it should work...
	
	return res;
}

vector< Move > HistoryHandler::getToPreviousTurn() const {

}

void HistoryHandler::registerMoveDone (const Move& move) {
	//TODO
	emit moveDone(move);
}

void HistoryHandler::registerMoveUndone() {
	//TODO
	Move move;	//TODO assign the move
	emit moveUndone(move);
}
