/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "../DiaballikEngine/src/Move.h"

class Player : public QObject {
	Q_OBJECT;
	
	protected:
		bool moveReady, ready, turnFinished;
		Move move;
	public:
		Player();
		
		bool isMoveReady();
		bool isReady();
		bool isTurnFinished();
		/**
		 * @brief Returns the next move if it's ready
		 **/
		const Move getMove();
		
		virtual void endGame(bool win) = 0;
	public slots:
		/**
		 * @brief Tells the player to finish current turn
		 **/
		void finishTurn();
    };

#endif // PLAYER_H
