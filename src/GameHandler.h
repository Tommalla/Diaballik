/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <QObject>
#include <QTimer>
#include <vector>
#include "../DiaballikEngine/src/Game.h"
#include "../DiaballikEngine/src/Singleton.h"
#include "GraphicsScene.h"
#include "GraphicsTile.h"
#include "GraphicsMovableTile.h"
#include "PlayerInfo.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "gameEnums.h"

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
		
		vector<Move> currentTurn;
		vector< vector<Move> > turnsHistory;
		
		vector<GraphicsTile*> backgroundTiles;
		vector<GraphicsMovableTile*> pawns;
		vector<GraphicsMovableTile*> balls;
		vector<GraphicsTile*> selectedTiles;
		GraphicsMovableTile* lastSelector;	//the last tile that was clicked and became
		//the source of the last selection. If none, this shall be set to NULL.
		
		QTimer playersTimer;
		Player* players[2];
		int currentPlayer;
		
		void changeCurrentPlayer();
		void deletePlayers();
		Player* createPlayer(const PlayerInfo& info, const int id);
		
		GraphicsTile* getTileAt(const Point& pos);
		GraphicsMovableTile* getBallAt(const Point& pos);
		GraphicsMovableTile* getPawnAt(const Point& pos);
		
		/**
		 * @brief Deselects currently selected tiles.
		 **/
		void deselectTiles();
		GameHandler();
	public:
		/**
		 * @brief Initializes the class
		 * @param scene The GraphicsScene that is set in the view
		 **/
		void Initialize(GraphicsScene* scene);
		
		/**
		 * @brief Returns true if a move is valid
		 * @param src From coordinates
		 * @param dst To coordinates
		 **/
		bool isMoveValid(const Point& src, const Point& dst);
		/**
		 * @brief Attempts to move a tile. Returns true if a move is possible.
		 * @param src A pointer to the src (valid GraphicsMovableTile)
		 * @param dst A pointer to the dst (valid GraphicsMovableTile)
		 * @return true if the move succeeded, false if it's impossible
		 **/
		bool moveTile(const GraphicsMovableTile* src, const GraphicsMovableTile* dst);
		/**
		 * @brief Gets possible destinations for the tile to move/pass ball and
		 * selects them on the board (calling GraphicsTile::select on tiles)
		 * @return void
		 **/
		void showDestinationsFor(GraphicsMovableTile* tile);
		/**
		 * @brief Orders all the tiles to repaint.
		 **/
		void repaintTiles(QRect viewRect);
		
		bool canUndoMove();
		bool canRedoMove();
		void undoMove();
		void redoMove();
		
		/**
		 * @brief Returns the tile that is the source of the current selection.
		 * @return A pointer to the Tile. If there's no selection then the returned 
		 * value is NULL.
		 **/
		const GraphicsMovableTile* getLastSelector() const;
		
		/**
		 * @brief Starts a new game
		 *
		 * @param defaultConfig if set to true, GameHandler will create a new game on a standard board
		 * If set to false, GameHandler will attempt to read the configuration of pawns currently present 
		 * on GraphicsScene and start new game from this configuration (eg. after editing the board)
		 **/
		void newGame(const PlayerInfo& playerA, const PlayerInfo& playerB, 
			     QRect viewRect, bool defaultConfig = true);
	private slots:
		/**
		 * @brief Checks if the current player has yielded a move.
		 * If so - performs it and changes the active player. Stops the internal timer while it's performing
		 * the move.
		 **/
		void checkForNewMoves();
	signals:
		void gameFinished();
};

#endif // GAMEHANDLER_H
