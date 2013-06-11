/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"

class HumanPlayer : public Player {
	Q_OBJECT;
	public:
		HumanPlayer(const PlayerInfo &info);
		virtual void endGame(bool win);
		virtual void play(const vector<Move> moves);
		
	private slots:
		void setMove(const Move& move);
};

#endif // HUMANPLAYER_H
