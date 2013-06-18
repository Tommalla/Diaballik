/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"

class HumanPlayer : public Player {
	Q_OBJECT;
	public:
		HumanPlayer(const PlayerInfo &info);
};

#endif // HUMANPLAYER_H
