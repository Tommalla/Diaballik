/* Tomasz [Tommalla] Zakrzewski, 2013
 * All rights reserved */

#include <cassert>
#include "HistoryHandler.h"

vector< Move > HistoryHandler::getMovesBetween (int begin, int end) const {
	assert(end < (int)turns[turnId].size());
	vector<Move> res;
	
	for (int i = begin; i <= end; ++i)
		res.push_back(turns[turnId][i]);
	
	return res;
}

void HistoryHandler::dropTailAfter (const int turn, const int move) {
	while ((int)turns.size() > turn + 1)
		turns.pop_back();
	while ((int)turns[turn].size() > move + 1)
		turns[turn].pop_back();
}

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
	return lastMoveId >= 0 || turnId > 0; 
}

const bool HistoryHandler::canRedoMove() const {
	return lastMoveId + 1 < (int)turns[turnId].size() || 
		turnId + 1 < (int)turns.size();
}

const bool HistoryHandler::canUndoTurn() const {
	return canUndoMove();
}

const bool HistoryHandler::canRedoTurn() const {
	return canRedoMove();
}

Move HistoryHandler::getPreviousMove() const {
	assert(canUndoMove());
	
	return (canUndoMove()) ? (
		(lastMoveId >= 0) ? turns[turnId][lastMoveId] : 
			turns[turnId - 1][turns[turnId - 1].size() - 1]) :
		Move(-1, -1, -1, -1);	//should never happen
}

Move HistoryHandler::getNextMove() const {
	assert(canRedoMove());
	
	return (canRedoMove()) ? (
		(lastMoveId + 1 < (int)turns[turnId].size()) ? turns[turnId][lastMoveId + 1] :
			turns[turnId + 1][0]) :
		Move(-1, -1, -1, -1);
}

vector< Move > HistoryHandler::getToNextTurn() const {
	return getMovesBetween(lastMoveId + 1, turns[turnId].size() - 1);
}

vector< Move > HistoryHandler::getToPreviousTurn() const {
	return getMovesBetween(0, lastMoveId);
}

void HistoryHandler::registerMoveDone (const Move& move) {
	//prepare the indexes
	if (lastMoveId + 1 < (int)turns[turnId].size())
		++lastMoveId;
	else if (turnId + 1 < (int)turns.size() && turns[turnId + 1].empty() == false) {
		++turnId;
		lastMoveId = 0;
	} else {
		turns[turnId].push_back(Move(-1, -1, -1, -1));
		++lastMoveId;
	}
	
	if (turns[turnId][lastMoveId] != move)	//truncate the history
		dropTailAfter(turnId, lastMoveId);
	
	turns[turnId][lastMoveId] = move;
	
	emit moveDone(move);
}

void HistoryHandler::registerMoveUndone() {
	assert(canUndoMove());
	
	if (lastMoveId == -1)
		lastMoveId = turns[--turnId].size() - 1;
	
	emit moveUndone(turns[turnId][lastMoveId--]);
}
