/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "GameHandler.h"
#include "SettingsHandler.h"
#include "gameConstants.h"
#include "../DiaballikEngine/src/functions.h"

void GameHandler::changeCurrentPlayer() {
	this->currentPlayer = (this->currentPlayer + 1) % 2;
}

void GameHandler::deletePlayers() {
	for (int i = 0; i < 2; ++i) {
		this->players[i]->deleteLater();
		this->players[i] = NULL;
	}
}

Player* GameHandler::createPlayer (const PlayerInfo& info, const int id) {
	switch (info.type) {
		case HUMAN_PLAYER:
		{
			HumanPlayer* res = new HumanPlayer();
			
			for (GraphicsTile* tile: this->backgroundTiles)
				QObject::connect(tile, SIGNAL(makeMove(const Move&)), res, SLOT(setMove(const Move&)));
			
			FieldState field = (id == 0) ? PLAYER_A : PLAYER_B;
			for (GraphicsMovableTile* tile: this->movableTiles)
				if (this->game.getFieldAt(tile->getPos()) == field)
					QObject::connect(tile, SIGNAL(makeMove(const Move&)), res, SLOT(setMove(const Move&)));
			
			return res;
			break;
		}
		case AI_PLAYER:
		{
			//AIPlayer* res;
			//TODO create new AIPlayer - need AI implementation first!
			break;
		}
	}
}

void GameHandler::deselectTiles() {
	for (GraphicsTile* tile: this->selectedTiles)
		tile->deselect();
	this->selectedTiles.clear();
}


GameHandler::GameHandler() : QObject() {
	this->initialized = false;
	this->lastSelector = NULL;
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

void GameHandler::showDestinationsFor (GraphicsMovableTile* tile) {
	this->deselectTiles();
	
	vector<Point> destinations = this->game.getDestinationsFor(tile->getPos());
	FieldState field = this->game.getFieldAt(tile->getPos());
	
	for (Point dst: destinations)
		if (field == PLAYER_A || field == PLAYER_B) {
			for (GraphicsTile* tile: this->backgroundTiles)
				if (dst == tile->getPos()) {
					tile->select();
					this->selectedTiles.push_back(tile);
					break;
				}
		} else {
			for (GraphicsMovableTile* tile: this->movableTiles)
				if (dst == tile->getPos()) {
					tile->select();
					this->selectedTiles.push_back(tile);
					break;
				}
		}
		
	this->lastSelector = tile;
}

void GameHandler::repaintTiles (QRect viewRect) {
	int tileSize = min(viewRect.width(), viewRect.height()) / 7;
	
	for (GraphicsTile* tile: this->backgroundTiles)
		tile->redraw(tileSize, tileSize);
	
	for (GraphicsTile* tile: this->movableTiles)
		tile->redraw(tileSize, tileSize);
}

const GraphicsMovableTile* GameHandler::getLastSelector() const {
	return this->lastSelector;
}

void GameHandler::newGame (const PlayerInfo& playerA, const PlayerInfo& playerB,
			   QRect viewRect, bool defaultConfig) {
	this->lastSelector = NULL;
	
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
		
		this->deletePlayers();
		this->currentPlayer = 0;
		
		this->players[0] = this->createPlayer(playerA, 0);
		this->players[1] = this->createPlayer(playerB, 1);
		
		//TODO - random player starting?
	} else {
		//we're using the configuration from the scene
		//TODO create a new Board object and set everything on it as it's on the Scene
		//TODO check if the Board is valid and start the game
	}
}

void GameHandler::checkForNewMoves() {
	//TODO implement
}



