/* Tomasz [Tommalla] Zakrzewski, 2013
* All rights reserved */

#ifndef HISTORYHANDLER_H
#define HISTORYHANDLER_H

#include <QObject>
#include "../../DiaballikEngine/src/Move.h"
#include "../../DiaballikEngine/src/enums.h"
#include <vector>

using namespace std;

class HistoryHandler : public QObject {
	Q_OBJECT;
	private:
		GamePlayer currentPlayer;
		vector<vector<Move> > turns;
		int turnId, lastMoveId;
		
		inline vector<Move> getMovesBetween(int begin, int end) const;
		inline void dropTailAfter(const int turn, const int move);
	public:
		HistoryHandler(const GamePlayer& startingPlayer);
		HistoryHandler(const vector< vector< Move > >& turns, const int turnId, const int lastMoveId, const GamePlayer& startingPlayer);
		
		inline const bool canUndoMove() const;
		inline const bool canRedoMove() const;
		inline const bool canUndoTurn() const;
		inline const bool canRedoTurn() const;
		
		inline Move getPreviousMove() const;
		inline Move getNextMove() const;
		
		/**
		 * @brief Returns a vector of moves needed to go back to the last turn.
		 */
		inline vector<Move> getToPreviousTurn() const;
		/**
		 * @brief Returns a vector of moves needed to get to the next turn.
		 */
		inline vector<Move> getToNextTurn() const;
		
		void registerMoveDone(const Move& move);
		void registerMoveUndone();
		
		void finishTurn();
	signals:
		void turnDone(const GamePlayer& player, const vector<Move>& moves);
		void turnUndone(const GamePlayer& player, const vector<Move>& moves);
};

#endif // HISTORYHANDLER_H
