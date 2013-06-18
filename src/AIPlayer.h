/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <QProcess>
#include <queue>
#include "Player.h"

class AIPlayer : public Player {
	private:
		QProcess bot;
		bool processing;
		queue<Move> movesQueue;
	public:
		AIPlayer (const PlayerInfo& info);
		virtual bool isMoveReady();
		virtual const Move getMove();
		virtual void play (const GamePlayer& player, const vector<Move>& moves);
		void genMove();
		virtual void undoTurn (const GamePlayer& player, const vector<Move>& moves);
		virtual void endGame (bool win);
		virtual void finishTurn();
		virtual void startTurn();
		virtual void setMove (const Move& move);
		
	signals:
		void crashed();	//TODO: add handling in GameHandler and MainWindow
	
};

#endif // AIPLAYER_H
