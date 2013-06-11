/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include "GameHandler.h"
#include "SettingsHandler.h"
#include "gameConstants.h"
#include "../DiaballikEngine/src/functions.h"

void GameHandler::changeCurrentPlayer() {
	this->deselectTiles();
	for (int i = 0; i < 2; ++i)
		this->players[i]->play(this->currentTurn);
	this->currentPlayer = (this->currentPlayer + 1) % 2;
	this->turnsHistory.push_back(this->currentTurn);
	this->currentTurn.clear();
	this->game.setCurrentPlayer(engine::getOppositePlayer(this->game.getCurrentPlayer()));
	qDebug("Next player! %s", (this->game.getCurrentPlayer() == GAME_PLAYER_A) ? "A": "B");
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
			HumanPlayer* res = new HumanPlayer(info);
			
			for (GraphicsTile* tile: this->backgroundTiles)
				QObject::connect(tile, SIGNAL(makeMove(const Move&)), res, SLOT(setMove(const Move&)));
			
			GamePlayer fieldPlayer = (id == 0) ? GAME_PLAYER_A : GAME_PLAYER_B;
			for (GraphicsMovableTile* tile: this->pawns)
				if (engine::getPlayerFor(this->game.getFieldAt(tile->getPos())) == fieldPlayer)
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

GraphicsMovableTile* GameHandler::getPawnAt (const Point& pos) {
	for (GraphicsMovableTile* tile: this->pawns)
		if (tile->getPos() == pos)
			return tile;
	return NULL;
}

GraphicsMovableTile* GameHandler::getBallAt (const Point& pos) {
	for (GraphicsMovableTile* tile: this->balls)
		if (tile->getPos() == pos)
			return tile;
	return NULL;
}

GameHandler::GameHandler() : QObject() {
	this->initialized = false;
	this->lastSelector = NULL;
	
	this->playersTimer.setInterval(
		SettingsHandler::getInstance().value("application/playersTimerInterval", DEFAULT_PLAYERS_TIMER_INTERVAL).toInt() );
	this->playersTimer.stop();
	
	QObject::connect(&(this->playersTimer), SIGNAL(timeout()), this, SLOT(checkForNewMoves()));
}

void GameHandler::deselectTiles() {
	for (GraphicsTile* tile: this->selectedTiles)
		tile->deselect();
	this->selectedTiles.clear();
	if (this->lastSelector != NULL)
		this->lastSelector->deselect();
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
	if (this->game.getMovesLeft() <= 0 && this->game.getPassessLeft() <= 0)
		return;
	
	FieldState field = this->game.getFieldAt(tile->getPos());
	if (engine::getPlayerFor(field) != this->game.getCurrentPlayer())
		return;	//wrong player, we won't be selecting anything
	
	this->deselectTiles();
	
	vector<Point> destinations = this->game.getDestinationsFor(tile->getPos());
	
	for (Point dst: destinations)
		if (field == PLAYER_A || field == PLAYER_B) {
			if (this->game.getMovesLeft() <= 0)
				return;
			qDebug("background being selected!");
			for (GraphicsTile* dstTile: this->backgroundTiles)
				if (dst == dstTile->getPos()) {
					dstTile->select();
					this->selectedTiles.push_back(dstTile);
					break;
				}
		} else {
			if (this->game.getPassessLeft() <= 0)
				return;
			qDebug("pawns being selected!");
			for (GraphicsTile* dstTile: this->pawns)
				if (dst == dstTile->getPos()) {
					dstTile->select();
					this->selectedTiles.push_back(dstTile);
					break;
				}
		}
		
	this->lastSelector = tile;
	tile->select(false);
	
	if (field == BALL_A || field == BALL_B)	//a hack to get the ball to cancel the selection
		for(GraphicsTile* ball: this->balls)
			if (ball->getPos() == tile->getPos()) {
				ball->select(false);
				this->selectedTiles.push_back(ball);
				qDebug("selected the ball");
				break;
			}
}

void GameHandler::repaintTiles (QRect viewRect) {
	int tileSize = min(viewRect.width(), viewRect.height()) / 7;
	
	for (GraphicsTile* tile: this->backgroundTiles)
		tile->redraw(tileSize, tileSize);
	
	for (GraphicsTile* tile: this->pawns)
		tile->redraw(tileSize, tileSize);
	
	for (GraphicsTile* tile: this->balls)
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
		
		SettingsHandler::getInstance().sync();
		
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
		this->pawns.clear();
		this->selectedTiles.clear();
		this->balls.clear();
	
		//creating new board:
		this->game = Game(GAME_PLAYER_A);
		int tileSize = min(viewRect.width(), viewRect.height()) / 7;
		
		for (int x = 0; x < 7; ++x) {
			for (int y = 0; y < 7; ++y) {
				this->backgroundTiles.push_back(new GraphicsTile(backgroundTilePath, x, y, -1000, tileSize, tileSize));
				this->scene->addItem(this->backgroundTiles.back());
			}
		
			this->pawns.push_back(new GraphicsMovableTile(blackPawnPath, x, 0, 0, tileSize, tileSize));
			this->scene->addItem(this->pawns.back());
			this->pawns.push_back(new GraphicsMovableTile(whitePawnPath, x, 6, 0, tileSize, tileSize));
			this->scene->addItem(this->pawns.back());
		}
		
		this->balls.push_back(new GraphicsMovableTile(ballPath,	3, 0, 1, tileSize, tileSize));
		this->scene->addItem(this->balls.back());
		this->balls.push_back(new GraphicsMovableTile(ballPath, 3, 6, 1, tileSize, tileSize));
		this->scene->addItem(this->balls.back());
		
		this->deletePlayers();
		this->currentPlayer = 0;
		
		this->players[0] = this->createPlayer(playerA, 0);
		this->players[1] = this->createPlayer(playerB, 1);
		
		//TODO - random player starting?
		this->playersTimer.start();
	} else {
		//we're using the configuration from the scene
		//TODO create a new Board object and set everything on it as it's on the Scene
		//TODO check if the Board is valid and start the game
	}
}

void GameHandler::checkForNewMoves() {
	if (this->players[this->currentPlayer]->isTurnFinished()) {
		this->changeCurrentPlayer();
		//TODO has to notify the opposite player of the last move (turn)
	}
	
	if (this->players[this->currentPlayer]->isMoveReady()) {
		this->playersTimer.stop();
		
		Move move = this->players[this->currentPlayer]->getMove();
		
		if (this->game.isMoveValid(move) ) {
			qDebug("Valid move!");
			FieldState field = this->game.getFieldAt(move.from);
			GraphicsMovableTile* src;
			
			if (field == BALL_A || field == BALL_B)
				src = this->getBallAt(move.from);
			else
				src = this->getPawnAt(move.from);
			
			assert(src != NULL);
			
			this->deselectTiles();
			src->move(move.to);
			
			this->currentTurn.push_back(move);
			this->game.makeMove(move);
		}
		
		/*if (this->game.getMovesLeft() <= 0 && this->game.getPassessLeft() <= 0)
			this->changeCurrentPlayer();*/
	
		//FIXME: Needs debugging...
		if (this->game.isFinished()) {
			qDebug("Game finished! (This is going to need some debugging...");
 			emit gameFinished();
			return;
		}
		
		this->playersTimer.start();
	}
}

void GameHandler::currentTurnDone() {
	if (this->currentTurn.empty())
		return;	//disallow for empty turns

	this->deselectTiles();
	this->players[this->currentPlayer]->finishTurn();
}



