/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <QObject>
#include "../DiaballikEngine/src/Game.h"
#include "../DiaballikEngine/src/Singleton.h"
#include "GraphicsScene.h"
#include "GraphicsTile.h"


/**
 * @brief The main class of the application
 * Handles everything game-related - from creating and managing everything that is visible on the Scene
 * to saving/loading the game, beginning the new one, initializing players etc.
 * 
 * @warning You cannot use this class without calling Initalize(GraphicsScene*) first!
 **/
class GameHandler : public QObject, public Singleton<GameHandler> {
	Q_OBJECT;
	friend class Singleton<GameHandler>;
	private:
		bool initialized;
		GraphicsScene* scene;
		Game game;
		
		GameHandler();
	public:
		void Initialize(GraphicsScene* scene);
		
		/**
		 * @brief Attempts to move a tile. Returns true if a move is possible.
		 * @param tile A pointer to the caller (valid GraphicsTile)
		 * @return true if the move succeeded, false if it's impossible
		 **/
		bool moveTile(const GraphicsTile* tile);
	public slots:
		/**
		 * @brief Starts a new game
		 *
		 * @param defaultConfig if set to true, GameHandler will create a new game on a standard board
		 * If set to false, GameHandler will attempt to read the configuration of pawns currently present 
		 * on GraphicsScene and start new game from this configuration (eg. after editing the board)
		 **/
		void newGame(bool defaultConfig = true);
};

#endif // GAMEHANDLER_H
