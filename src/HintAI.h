/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef HINTAI_H
#define HINTAI_H
#include "../DiaballikEngine/src/Game.h"

/**
 * @brief A simple class to return hints
 **/
class HintAI {
	private:
		Game currentGame;
		vector<Move> movesMade;
		int movesAvailable[2];
		vector<vector<Move> > allMovesAvailable;
		
		const bool matches (const vector<Move>& a, const vector<Move>& b) const;
		const int score(const vector<Move>& turn);
	public:
		HintAI(const vector<Point>& black, const vector<Point>& white, const vector<Point>& ball, const GamePlayer& player);
		
		void gameChanged(const Game& newGame);
		/**
		 * @brief Generates a hint that matches the current pattern
		 *
		 * @param pattern A set of moves already done in this turn
		 * @return const vector< Move >
		 **/
		const vector<Move> generateHint(const vector<Move>& pattern);
		void calculateAvailableMovesFor (const Game& tmpGame);
};

#endif // HINTAI_H
