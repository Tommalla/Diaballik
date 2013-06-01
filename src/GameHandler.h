/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <QObject>
#include <vector>
#include "../DiaballikEngine/src/Game.h"
#include "../DiaballikEngine/src/Singleton.h"
#include "GraphicsScene.h"
#include "GraphicsTile.h"
#include "GraphicsMovableTile.h"

using namespace std;

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
		
		vector<GraphicsTile*> backgroundTiles;
		vector<GraphicsMovableTile*> movableTiles;
		vector<GraphicsTile*> selectedTiles;
		
		GraphicsTile* getTileAt(const Point& pos);
		GraphicsMovableTile* getMovableTileAt(const Point& pos);
		
		GameHandler();
	public:
		void Initialize(GraphicsScene* scene);
		
		/**
		 * @brief Attempts to move a tile. Returns true if a move is possible.
		 * @param tile A pointer to the caller (valid GraphicsTile)
		 * @return true if the move succeeded, false if it's impossible
		 **/
		bool moveTile(const GraphicsMovableTile* src, const GraphicsMovableTile* dst);
		/**
		 * @brief Gets possible destinations for the tile to move/pass ball and
		 * selects them on the board (calling GraphicsTile::select on tiles)
		 * @param tile 
		 * @return void
		 **/
		void showDestinationsFor(const GraphicsTile* tile);
		/**
		 * @brief Orders all the tiles to repaint.
		 **/
		void repaintTiles(QRect viewRect);
		
		bool canUndoMove();
		bool canRedoMove();
		void undoMove();
		void redoMove();
	public slots:
		/**
		 * @brief Starts a new game
		 *
		 * @param defaultConfig if set to true, GameHandler will create a new game on a standard board
		 * If set to false, GameHandler will attempt to read the configuration of pawns currently present 
		 * on GraphicsScene and start new game from this configuration (eg. after editing the board)
		 **/
		void newGame(QRect viewRect, bool defaultConfig = true);
	signals:
		void gameFinished();
};

#endif // GAMEHANDLER_H
