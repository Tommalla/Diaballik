/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"
#include <QObject>

class HumanPlayer : protected Player, protected QObject {
	Q_OBJECT;
	public:
		HumanPlayer();
		void endGame(bool win);
		
	private slots:
		void setMove(const Move& move);
};

#endif // HUMANPLAYER_H
