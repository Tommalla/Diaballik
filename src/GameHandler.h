/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <QObject>
#include <QSet>
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
		QSet<QString> hashes;
		
		vector< vector<Move> > turnsHistory;
		vector< pair<uint8_t, uint8_t> > movesLeft;
		vector< QString > hashesHistory;
		int lastMoveId, currentTurnId;
		/**
		 * @brief Deletes all the turns and moves remembered after lastMoveId.
		 **/
		void dropHistoryTail();
		
		vector<GraphicsTile*> backgroundTiles;
		vector<GraphicsMovableTile*> pawns;
		vector<GraphicsMovableTile*> balls;
		vector<GraphicsTile*> selectedTiles;
		GraphicsMovableTile* lastSelector;	//the last tile that was clicked and became
		//the source of the last selection. If none, this shall be set to NULL.
		
		QTimer playersTimer;
		Player* players[2];
		int currentPlayer;
		
		const int getNextPlayerId() const;
		void changeCurrentPlayer(const bool undo = false);
		void deletePlayers();
		Player* createPlayer(const PlayerInfo& info, const int id);
		
		GraphicsTile* getTileAt(const Point& pos);
		GraphicsMovableTile* getBallAt(const Point& pos);
		GraphicsMovableTile* getPawnAt(const Point& pos);
		GraphicsMovableTile* getSource(const Move& move);
		
		/**
		 * @brief Creates the visible board and related internal components
		 *
		 * @param tileSize The size of a single tile
		 * @param pawns An array of vectors of positions of the pawns (black, white)
		 * @param balls A vector of balls (black, white)
		 **/
		void createSceneBoard(const int tileSize, const vector<Point> pawns[2], const vector<Point> balls);
		
		GameHandler();
	public:
		/**
		 * @brief Deselects currently selected tiles.
		 **/
		void deselectTiles();
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
		 * @brief Attempts to move a tile.
		 **/
		void moveTile(const Move& move);
		/**
		 * @brief Moves the tile (without animation)
		 **/
		void changeTilePosition(const Move& move);
		/**
		 * @brief Gets possible destinations for the tile to move/pass ball and
		 * selects them on the board (calling GraphicsTile::select on tiles)
		 * @return void
		 **/
		void showDestinationsFor(GraphicsMovableTile* tile);
		/**
		 * @brief Orders all the tiles to repaint.
		 **/
		void repaintTiles(const int tileSize);
		
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
			     const int tileSize, bool defaultConfig = true);
		/**
		 * @brief If possible, loads the game from the save (filename).
		 *
		 * @param filename The path to the save file.
		 * @return bool True if the load succeded.
		 **/
		bool loadGame(const QString filename, const PlayerInfo& playerA, const PlayerInfo& playerB, 
			      const int tileSize);
		/**
		 * @brief If possible, saves the current game to filename.
		 *
		 * @param filename The path to the desired savegame file.
		 * @return bool True if save succeded.
		 **/
		bool saveGame(const QString filename) const;
		/**
		 * @brief Returns the name of the player
		 * 
		 * @param current If set to true, the method shall return the name of the current player. Otherwise
		 * it will return the name of the next player.
		 * 
		 * @return The name described. An empty QString if there is any error (eg. players are nonexistent).
		 **/
		const QString& getPlayerName(const bool current = true) const;
		const QString getWinnerName() const;
		
		const bool canUndoMove() const;
		const bool canRedoMove() const;
		const bool canUndoTurn() const;
		const bool canRedoTurn() const;
		
		/**
		 * @brief Returns how many moves are left for the current player
		 **/
		const int getMovesLeft() const;
		/**
		 * @brief Returns how many passes are left for the current player
		 **/
		const int getPassesLeft() const;
	private slots:
		/**
		 * @brief Checks if the current player has yielded a move.
		 * If so - performs it and changes the active player. Stops the internal timer while it's performing
		 * the move.
		 **/
		void checkForNewMoves();
	public slots:
		/**
		 * @brief Finishes current turn and gives control to the next player.
		 **/
		void currentTurnDone();
		void undoMove();
		void redoMove();
		void undoTurn();
		void redoTurn();
	signals:
		void playerChanged();
		void gameFinished();
		void moveFinished();
};

#endif // GAMEHANDLER_H
