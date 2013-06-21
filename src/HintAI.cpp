/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "HintAI.h"
#include "../DiaballikEngine/src/functions.h"

const bool HintAI::matches (const vector< Move >& a, const vector< Move >& b) const {
	if (a.size() >= b.size())
		return false;
	for (int i = 0; i < (int)a.size(); ++i)
		if (a[i] != b[i])
			return false;
	return true;
}

const int HintAI::score (const vector< Move >& turn) {
	int res = 0;
	Game tmpGame = this->currentGame;
	for (Move move: turn)
		tmpGame.makeMove(move);
		
	for (int x = 0; x < 7; ++x)
		for (int y = 0; y < 7; ++y)
			if (engine::getPlayerFor(tmpGame.getFieldAt(x, y)) == tmpGame.getCurrentPlayer()) {
				int d = (tmpGame.getCurrentPlayer() == GAME_PLAYER_A) ? y : 6 - y;
		
			if (d == 6) 
				d += 20;
			res += 10 * d;
		}
		
	if (tmpGame.isFinished())	//we won
		res += 900;
	else {
		if (engine::canWinInOneTurn(tmpGame, engine::getOppositePlayer(tmpGame.getCurrentPlayer())))
			res -= 1000;	//the opponent can win in one turn - we don't want that
		
		if (engine::canWinInOneTurn(tmpGame, tmpGame.getCurrentPlayer()))	//theoretically speaking we can win in one turn
			res += 450;	//but the opponent's turn still remains to be done (which can change things)
	}
		
	return res;
}


HintAI::HintAI (const vector< Point >& black, const vector< Point >& white, const vector< Point >& ball, const GamePlayer& player) {
	this->currentGame.newGame(black, white, ball);
	this->currentGame.setCurrentPlayer(player);
	
	this->movesAvailable[0] = 2;
	this->movesAvailable[1] = 1;
	
	this->allMovesAvailable.clear();
	this->calculateAvailableMovesFor(this->currentGame);
}

void HintAI::gameChanged (const Game& newGame) {
	this->currentGame = newGame;
	this->currentGame.setCurrentPlayer(newGame.getCurrentPlayer(), 2, 1);
	
	this->allMovesAvailable.clear();
	this->calculateAvailableMovesFor(this->currentGame);
}

const vector< Move > HintAI::generateHint (const vector< Move >& pattern) {
	vector<Move> res;
	int val = -1000000;
	
	for (vector<Move> turn: this->allMovesAvailable)
		if (this->matches(pattern, turn)) {
			int tmp = this->score(turn);
			if (tmp >= val) {
				val = tmp;
				res = turn;
			}
		}
	return res;
}

void HintAI::calculateAvailableMovesFor (const Game& tmpGame) {
	vector<Point> pawns;
	vector<Point> destinations;
	
	pawns = tmpGame.getPawnsOf(tmpGame.getCurrentPlayer());	//get all pawns of current player
	
	for (Point pawn: pawns) {	//try to move every pawn
		FieldState src = tmpGame.getFieldAt(pawn);
		
		for (int i = 0; i < 2; ++i)	//for each pawn, try move or ball pass
			if (this->movesAvailable[i] > 0 && ( 
				(i == 0 && (src == PLAYER_A || src == PLAYER_B)) ||
				(i == 1 && (src == BALL_A || src == BALL_B)))) {	//try to make a move
				destinations = tmpGame.getDestinationsFor(pawn);
			
			for (Point dst: destinations) {	//for every destination available
				Game tmp2 = tmpGame;	//generate new game
				tmp2.makeMove(pawn, dst);	//with that move made
				
				this->movesMade.push_back(Move(pawn, dst));	//add move to queue
				
				//proceed with the recursion
				this->movesAvailable[i]--;
				allMovesAvailable.push_back(this->movesMade);
				
				if ((this->movesAvailable[0] > 0 || this->movesAvailable[1] > 0) &&
					!tmp2.isFinished())
					this->calculateAvailableMovesFor(tmp2);
				
				this->movesAvailable[i]++;
				this->movesMade.pop_back();	//remove move from queue
			}
		}
	}
}



