/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "GameHandler.h"
#include "SettingsHandler.h"
#include "gameConstants.h"

void GameHandler::changeCurrentPlayer() {
	this->currentPlayer = (this->currentPlayer + 1) % 2;
}


GameHandler::GameHandler() : QObject() {
	this->initialized = false;
}

void GameHandler::Initialize (GraphicsScene* scene) {
	this->scene = scene;
	this->initialized = true;
}

bool GameHandler::isMoveValid (const Point& src, const Point& dst) {
	return this->game.isMoveValid(src, dst);
}


bool GameHandler::moveTile (const GraphicsMovableTile* src, const GraphicsMovableTile* dst) {
	//TODO check if the move is valid
	//if so-make it
	//return true/false
}

void GameHandler::repaintTiles (QRect viewRect) {
	int tileSize = min(viewRect.width(), viewRect.height()) / 7;
	
	for (GraphicsTile* tile: this->backgroundTiles)
		tile->redraw(tileSize, tileSize);
	
	for (GraphicsTile* tile: this->movableTiles)
		tile->redraw(tileSize, tileSize);
}


void GameHandler::newGame (QRect viewRect, bool defaultConfig) {
	this->lastSelector = NULL;
	
	this->currentPlayer = 0;
	//TODO initialize players, connect human players etc.
	
	if (defaultConfig == true) {
		qDebug("Creating new game from the scratch");
		//creating a default board
		
		//FIXME Don't know QSettings implementation, BUT if it reads the settings again every time then having this here
		//is a good thing - a user can change the graphics in the config and have a "modded" game just by starting a new game;
		//otherwise (eg. if it's not the case), this should be moved to gameConstants:
		//reading tiles paths from config
		QString backgroundTilePath, whitePawnPath, blackPawnPath, ballPath;
		
		backgroundTilePath = whitePawnPath = blackPawnPath = ballPath = APP_LOCATION + "graphics/";
		
		backgroundTilePath += SettingsHandler::getInstance().value("graphics/backgroundTile", "background.png").toString();
		whitePawnPath += SettingsHandler::getInstance().value("graphics/whitePawn", "white.png").toString();
		blackPawnPath += SettingsHandler::getInstance().value("graphics/blackPawn", "black.png").toString();
		ballPath += SettingsHandler::getInstance().value("graphics/ball", "ball.png").toString();
		
		qDebug("Paths:\n%s\n%s\n%s\n%s", backgroundTilePath.toStdString().c_str(), whitePawnPath.toStdString().c_str(),
		       blackPawnPath.toStdString().c_str(), ballPath.toStdString().c_str());
	
		//cleaning the board:
		this->scene->clear();
		this->backgroundTiles.clear();
		this->movableTiles.clear();
		this->selectedTiles.clear();
	
		//creating new board:
		this->game = Game();
		int tileSize = min(viewRect.width(), viewRect.height()) / 7;
		
		for (int x = 0; x < 7; ++x) {
			for (int y = 0; y < 7; ++y) {
				this->backgroundTiles.push_back(new GraphicsTile(backgroundTilePath, x, y, tileSize, tileSize));
				this->scene->addItem(this->backgroundTiles.back());
			}
		
			this->movableTiles.push_back(new GraphicsMovableTile(blackPawnPath, x, 0, tileSize, tileSize));
			this->scene->addItem(this->movableTiles.back());
			this->movableTiles.push_back(new GraphicsMovableTile(whitePawnPath, x, 6, tileSize, tileSize));
			this->scene->addItem(this->movableTiles.back());
			
			if (x == 3) {
				this->movableTiles.push_back(new GraphicsMovableTile(ballPath, x, 0, tileSize, tileSize));
				this->scene->addItem(this->movableTiles.back());
				this->movableTiles.push_back(new GraphicsMovableTile(ballPath, x, 6, tileSize, tileSize));
				this->scene->addItem(this->movableTiles.back());
			}
		}
	} else {
		//we're using the configuration from the scene
		//TODO create a new Board object and set everything on it as it's on the Scene
		//TODO check if the Board is valid and start the game
	}
}

void GameHandler::checkForNewMoves() {
	//TODO implement
}



