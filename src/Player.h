/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef PLAYER_H
#define PLAYER_H

#include "../DiaballikEngine/src/Move.h"

class Player {
	protected:
		bool moveReady, ready;
		Move move;
	public:
		Player();
		
		bool isMoveReady();
		bool isReady();
		Move& getMove();
		
		virtual void endGame(bool win) = 0;
    };

#endif // PLAYER_H
