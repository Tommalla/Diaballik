/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Player.h"

class AIPlayer : public Player {
	public:
		AIPlayer (const PlayerInfo& info);
		virtual bool isMoveReady();
		virtual void play (const std::vector< Move, std::allocator< Move > > moves);
		void genMove();
		virtual void undoTurn (const std::vector< Move, std::allocator< Move > > moves);
		virtual void endGame (bool win);
		virtual void finishTurn();
		virtual void startTurn();
		virtual void setMove (const Move& move);
};

#endif // AIPLAYER_H
