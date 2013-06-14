/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <vector>
#include "../DiaballikEngine/src/Move.h"
#include "PlayerInfo.h"

using namespace std;

class Player : public QObject {
	Q_OBJECT;
	
	protected:
		PlayerInfo info;
		bool moveReady, ready, turnFinished;
		Move move;
	public:
		Player(const PlayerInfo& info);
		
		bool isMoveReady();
		bool isReady();
		bool isTurnFinished();
		/**
		 * @brief Returns the next move if it's ready
		 **/
		const Move getMove();
		
		const PlayerInfo& getPlayerInfo() const;
		
		/**
		 * @brief A method that is called to pass the opponent's (and own) moves to the player
		 *
		 * @param moves The moves that the game accepted and performed.
		 **/
		virtual void play(const vector<Move> moves) = 0;
		/**
		 * @brief Tells the player to undo a turn
		 *
		 **/
		virtual void undoTurn(const vector<Move> moves);
		virtual void endGame(bool win) = 0;
	public slots:
		/**
		 * @brief Tells the player to finish current turn
		 **/
		virtual void finishTurn();
		/**
		 * @brief Tells the player to start a new turn
		 **/
		virtual void startTurn();
		virtual void setMove(const Move& move);
    };

#endif // PLAYER_H
