/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef PLAYER_H
#define PLAYER_H

#include "Move.h"

class Player {
	protected:
		bool moveReady, ready;
		Move move;
	public:
		Player();
		
		bool isMoveReady();
		bool isReady();
		Move& getMove();
		
		virtual void endGame(bool win);
    };

#endif // PLAYER_H
