/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"

class HumanPlayer : public Player {
	Q_OBJECT;
	public:
		HumanPlayer();
		virtual void endGame(bool win);
		
	private slots:
		void setMove(const Move& move);
};

#endif // HUMANPLAYER_H
