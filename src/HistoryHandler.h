/* Tomasz [Tommalla] Zakrzewski, 2013
* All rights reserved */

#ifndef HISTORYHANDLER_H
#define HISTORYHANDLER_H

#include <QObject>
#include "../../DiaballikEngine/src/Move.h"
#include <vector>

using namespace std;

class HistoryHandler : public QObject {
	Q_OBJECT;
	private:
		vector<vector<Move> > turns;
		int turnId, lastMoveId;
	public:
		HistoryHandler();
		HistoryHandler(const vector< vector< Move > >& turns, const int turnId, const int lastMoveId);
		
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
	signals:
		void moveDone(const Move& move);
		void moveUndone(const Move& move);
};

#endif // HISTORYHANDLER_H
