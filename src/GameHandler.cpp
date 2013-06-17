/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include <QDebug>
#include "GameHandler.h"
#include "SettingsHandler.h"
#include "gameConstants.h"
#include "SaveHandler.h"
#include "../DiaballikEngine/src/functions.h"

void GameHandler::dropHistoryTail() {
	while (this->turnsHistory.size() > this->currentTurnId + 1)
		this->turnsHistory.pop_back();
	
	while (this->movesLeft.size() > this->currentTurnId + 1)
		this->movesLeft.pop_back();

	while (this->turnsHistory.back().size() > this->lastMoveId + 1)
		this->turnsHistory.back().pop_back();
}

const int GameHandler::getNextPlayerId() const {
	return (this->currentPlayer + 1) % PLAYERS_QTY;
}

void GameHandler::changeCurrentPlayer(const bool undo) {
	this->deselectTiles();
	
	this->players[this->getNextPlayerId()]->startTurn();
	this->players[this->currentPlayer]->finishTurn();
	
	if (!undo) {
		for (int i = 0; i < PLAYERS_QTY; ++i)
			this->players[i]->play(this->turnsHistory[this->currentTurnId]);

		if (this->turnsHistory.size() <= this->currentTurnId + 1)
			this->turnsHistory.push_back(vector<Move>());
		if (this->movesLeft.size() <= this->currentTurnId + 1)
			this->movesLeft.push_back(make_pair(2, 1));
		
		this->movesLeft[this->currentTurnId] = make_pair(this->game.getMovesLeft(), this->game.getPassessLeft());
		this->currentTurnId++;
		this->lastMoveId = -1;
	} else {
		if (this->currentTurnId > 0) {
			if (this->turnsHistory[this->currentTurnId].empty())
				this->turnsHistory.pop_back();
			
			this->movesLeft.pop_back();
			
			this->currentTurnId--;
			
			for (Player* player: this->players)
				player->undoTurn(this->turnsHistory[this->currentTurnId]);
			
			this->lastMoveId = this->turnsHistory[this->currentTurnId].size() - 1;
		} else {
			this->currentTurnId = 0;
			this->lastMoveId = -1;
		}
	}
	
	this->currentPlayer = this->getNextPlayerId();
	this->game.setCurrentPlayer(engine::getOppositePlayer(this->game.getCurrentPlayer()), 
		this->movesLeft[this->currentTurnId].first, this->movesLeft[this->currentTurnId].second);
	qDebug("Next player! %s", (this->game.getCurrentPlayer() == GAME_PLAYER_A) ? "A": "B");
	
	//hashes
	if (!undo) {
		//new configuration
		QString h = QString::fromStdString(this->game.getHash());
		
		if (this->hashes.contains(h)) {	//state repetition - draw
			this->game.setCurrentPlayer(NONE);
			emit gameFinished();
			return;
		}
		this->hashes.insert(h);
		this->hashesHistory.push_back(h);
	} else {
		this->hashes.remove(hashesHistory.back());
		this->hashesHistory.pop_back();
	}
	
	emit playerChanged();
}

void GameHandler::deletePlayers() {
	for (int i = 0; i < PLAYERS_QTY; ++i) {
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

void GameHandler::initializePlayers (const PlayerInfo& playerA, const PlayerInfo& playerB) {
	this->deletePlayers();
	
	this->players[0] = this->createPlayer(playerA, 0);
	this->players[1] = this->createPlayer(playerB, 1);
	
	this->players[this->currentPlayer]->startTurn();
	this->players[this->getNextPlayerId()]->finishTurn();
	this->playersTimer.start();
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

GraphicsMovableTile* GameHandler::getSource (const Move& move) {
	FieldState field = this->game.getFieldAt(move.from);
	GraphicsMovableTile* src;
	
	if (field == BALL_A || field == BALL_B)
		src = this->getBallAt(move.from);
	else
		src = this->getPawnAt(move.from);
	
	assert(src != NULL);
	return src;
}

void GameHandler::createSceneBoard (const int tileSize, const vector< Point > pawns[2], const vector< Point > balls) {
	//cleaning the board:
	this->scene->clear();
	this->backgroundTiles.clear();
	this->pawns.clear();
	this->selectedTiles.clear();
	this->balls.clear();
	
	SettingsHandler::getInstance().sync();
	
	QString backgroundTilePath, whitePawnPath, blackPawnPath, ballPath;
	backgroundTilePath = whitePawnPath = blackPawnPath = ballPath = APP_LOCATION + "graphics/";
	
	backgroundTilePath += SettingsHandler::getInstance().value("graphics/backgroundTile", "background.png").toString();
	whitePawnPath += SettingsHandler::getInstance().value("graphics/whitePawn", "white.png").toString();
	blackPawnPath += SettingsHandler::getInstance().value("graphics/blackPawn", "black.png").toString();
	ballPath += SettingsHandler::getInstance().value("graphics/ball", "ball.png").toString();
	
	qDebug("Paths:\n%s\n%s\n%s\n%s", backgroundTilePath.toStdString().c_str(), whitePawnPath.toStdString().c_str(),
	       blackPawnPath.toStdString().c_str(), ballPath.toStdString().c_str());
	
	QString pawnPaths[2] = {blackPawnPath, whitePawnPath};
	
	for (int x = 0; x < 7; ++x)
		for (int y = 0; y < 7; ++y) {
			this->backgroundTiles.push_back(new GraphicsTile(backgroundTilePath, x, y, -1000, tileSize, tileSize));
			this->scene->addItem(this->backgroundTiles.back());
		}	
	
	for (int i = 0; i < 2; ++i)
		for (Point pawn: pawns[i]) {
			this->pawns.push_back(new GraphicsMovableTile(pawnPaths[i], pawn.x, pawn.y,  0, tileSize, tileSize));
			this->scene->addItem(this->pawns.back());
		}
	
	for (Point ball: balls) {
		this->balls.push_back(new GraphicsMovableTile(ballPath, ball.x, ball.y, 1, tileSize, tileSize));
		this->scene->addItem(this->balls.back());
	}
}

GameHandler::GameHandler() : QObject() {
	this->initialized = false;
	this->lastSelector = NULL;
	this->players[0] = this->players[1] = NULL;
	
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


void GameHandler::moveTile (const Move& move) {
	GraphicsMovableTile* src = this->getSource(move);
	this->deselectTiles();
	src->move(move.to);
}

void GameHandler::changeTilePosition (const Move& move) {
	GraphicsMovableTile* src = this->getSource(move);
	this->deselectTiles();
	src->changePosition(move.to.x, move.to.y);
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

void GameHandler::repaintTiles (const int tileSize) {
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
			   const int tileSize, bool defaultConfig) {
	this->lastSelector = NULL;
	this->hashes.clear();
	this->hashesHistory.clear();
	
	if (defaultConfig == true) {
		qDebug("Creating new game from the scratch");
		//creating a default board
		this->game = Game(GAME_PLAYER_A);
		vector<Point> pawns[2];
		vector<Point> balls = {Point(3, 0), Point(3, 6)};
		
		for (int i = 0; i < 2; ++i)
			for (int x = 0; x < 7; ++x)
				pawns[i].push_back(Point(x, 6 * i));
		
		this->createSceneBoard(tileSize, pawns, balls);
	
	} else {
		//we're using the configuration from the scene
		//TODO create a new Board object and set everything on it as it's on the Scene
		//TODO check if the Board is valid and start the game
	}
	
	this->currentPlayer = 0;
	
	this->initializePlayers(playerA, playerB);
	
	this->lastMoveId = -1;
	this->currentTurnId = 0;
	this->turnsHistory = {vector<Move>()};
	this->movesLeft = {{2, 1}};
	this->hashes.insert(QString::fromStdString(this->game.getHash()));
}

bool GameHandler::loadGame (const QString filename, const PlayerInfo& playerA, const PlayerInfo& playerB, 
			    const int tileSize) {
	//TODO will need some player info
	qDebug("Loading game...");
	SaveHandler save(filename);
	if (!save.load())
		return false;
	
	vector< Point > figures = save.getFigures();
	vector< Point > pawns[2];
	vector< Point > balls;
	
	//divide the figures to black and white
	for (int i = 0; i < 2; ++i)
		for (int j = i * 7; j < (i + 1) * 7; ++j)
			pawns[i].push_back(figures[j]);
		
	balls = {figures[figures.size() - 2], figures.back()};
	if (this->game.newGame(pawns[0], pawns[1], balls) == false)
		return false;
	
	this->game.setCurrentPlayer((save.getPlayer() == 0) ? GAME_PLAYER_A : GAME_PLAYER_B);
	this->currentPlayer = save.getPlayer();
	this->turnsHistory = save.getHistory();
	qDebug("Player %d starting", this->currentPlayer);
	
	//validate moves from history
	Game tmpGame = this->game;
	int p = this->currentPlayer;
	
	//go back through the history and calculate the initial board
	for (int i = this->turnsHistory.size() - 1; i >= 0; --i) {
		for (int j = this->turnsHistory[i].size() - 1; j >= 0; --j) {
			Move move = this->turnsHistory[i][j];
			move.revert();
			
			if (tmpGame.isMovePossible(move) == false)
				return false;
			
			tmpGame.makeMove(move, true);
		}
		
		p = (p + 1) % 2;
		tmpGame.setCurrentPlayer((p == 0) ? GAME_PLAYER_A : GAME_PLAYER_B);
	}
	
	//back at the beginning, can start calculating now
	this->movesLeft.clear();
	this->hashes.clear();
	this->hashesHistory.clear();
	
	this->hashes.insert(QString::fromStdString(tmpGame.getHash()));
	//p = (p + 1) % 2;
	tmpGame.setCurrentPlayer((p == 0) ? GAME_PLAYER_A : GAME_PLAYER_B);
	
	for (vector<Move> moves: this->turnsHistory) {
		for (Move move: moves) {
			qDebug("%d %d -> %d %d", move.from.x, move.from.y, move.to.x, move.to.y);
			if (!tmpGame.isMoveValid(move))
				return false;
			tmpGame.makeMove(move);
		}
	
		movesLeft.push_back(make_pair(tmpGame.getMovesLeft(), tmpGame.getPassessLeft()));
	
		p = (p + 1) % 2;
		tmpGame.setCurrentPlayer((p == 0) ? GAME_PLAYER_A : GAME_PLAYER_B);
		QString h = QString::fromStdString(tmpGame.getHash());
		
		if (hashes.contains(h))
			return false;
		
		hashes.insert(h);
		hashesHistory.push_back(h);
	}
	
	qDebug("Player %d finally starting", p);
	
	//FIXME possibly buggy
	if (!this->turnsHistory.empty()) {
		this->currentTurnId = max(0, (int)this->turnsHistory.size() - 1);
		this->lastMoveId = this->turnsHistory.back().size() - 1;
	} else {
		this->currentTurnId = 0;
		this->lastMoveId = -1;
	}
	
	this->game = tmpGame;
	
	//board validated, history created, time to draw!
	this->createSceneBoard(tileSize, pawns, balls);
	this->initializePlayers(playerA, playerB);
	
	return true;
}

bool GameHandler::saveGame (const QString filename) const {
	qDebug("Saving game...");
	SaveHandler save(filename);
	
	vector<Point> figures;
	for (GraphicsTile* tile: this->pawns)
		figures.push_back(tile->getPos());
	for (GraphicsTile* ball: this->balls)
		figures.push_back(ball->getPos());
	
	vector< vector<Move> > history = this->turnsHistory;
	while (history.size() > this->currentTurnId + 1)
		history.pop_back();
	while (history.back().size() > this->lastMoveId + 1)
		history.back().pop_back();
	
	qDebug("Saving history of size: %d, move: %d", history.size(), history.back().size());
	
	if (!save.save(figures, this->currentPlayer, this->turnsHistory))
		return false;
	
	return true;
}


const QString& GameHandler::getPlayerName (const bool current) const {
	if (this->players[0] == NULL || this->players[1] == NULL)
		return "";
	return this->players[((current) ? this->currentPlayer : this->getNextPlayerId())]->getPlayerInfo().name;
}

const QString GameHandler::getWinnerName() const {
	if (this->game.isFinished() == false || this->game.getWinner() == NONE)
		return "";
	return this->getPlayerName();
}

const int GameHandler::getMovesLeft() const {
	return this->game.getMovesLeft();
}

const int GameHandler::getPassesLeft() const {
	return this->game.getPassessLeft();
}

void GameHandler::checkForNewMoves() {
	if (this->players[this->currentPlayer]->isTurnFinished())
		this->changeCurrentPlayer();
	
	if (this->players[this->currentPlayer]->isMoveReady()) {
		this->playersTimer.stop();
		
		Move move = this->players[this->currentPlayer]->getMove();
		
		if (this->game.isMoveValid(move) ) {
			qDebug("Valid move!");

			this->moveTile(move);
			
			//if the move was the next from the history, increase the index
			//if not, erase the end of the vector and start appending new moves
			//forgetting the move if it's not from history:
			if (this->lastMoveId + 1 < this->turnsHistory[this->currentTurnId].size() &&
				this->turnsHistory[this->currentTurnId][this->lastMoveId + 1] != move)	//a different move
				this->dropHistoryTail();
			
			this->lastMoveId++;
			if (this->turnsHistory[this->currentTurnId].size() == this->lastMoveId)
				this->turnsHistory[this->currentTurnId].push_back(move);
			
			this->game.makeMove(move);
			emit moveFinished();
		}
		
		//TODO make this a feature
		/*if (this->game.getMovesLeft() <= 0 && this->game.getPassessLeft() <= 0)
			this->changeCurrentPlayer();*/
	
		if (this->game.isFinished()) {
			qDebug("Game finished!");
 			emit gameFinished();
			return;
		}
		
		this->playersTimer.start();
	}
}

void GameHandler::currentTurnDone() {
	if (this->lastMoveId < 0)
		return;	//disallow for empty turns

	this->players[this->getNextPlayerId()]->startTurn();
	this->players[this->currentPlayer]->finishTurn();
}

void GameHandler::undoMove() {
	qDebug("undoMove() %d %d", this->lastMoveId, this->currentTurnId);

	if (this->lastMoveId < 0 && this->currentTurnId > 0) {
		//we cannot move back, have to switch player
		qDebug("Dupa");
		this->changeCurrentPlayer(true);
	} else if (this->lastMoveId < 0 && this->currentTurnId == 0)
		return;
	
	Move move = this->turnsHistory[this->currentTurnId][this->lastMoveId];
	move.revert();
	
	if (this->game.isMovePossible(move)) {
		qDebug("Undo possible!");
		this->moveTile(move);
		this->lastMoveId--;
		this->game.makeMove(move, true);
		emit moveFinished();
	} else qDebug("Undo impossible: %d %d -> %d %d", move.from.x, move.from.y, move.to.x, move.to.y);
}

void GameHandler::redoMove() {
	qDebug("redoMove() %d %d", this->lastMoveId, this->currentTurnId);
	//move to the current turn
	if (this->lastMoveId + 1 >= this->turnsHistory[this->currentTurnId].size() && 
		this->currentTurnId + 1 < this->turnsHistory.size())
		this->changeCurrentPlayer();
	else if (this->lastMoveId + 1 >= this->turnsHistory[this->currentTurnId].size() && 
		this->currentTurnId + 1 >= this->turnsHistory.size())
		return;
	
	qDebug("redo possible!");
	
	//send the move to the current player
	this->players[this->currentPlayer]->setMove(this->turnsHistory[this->currentTurnId][this->lastMoveId + 1]);
}

void GameHandler::undoTurn() {
	qDebug("undoTurn()");
	for (int i = this->lastMoveId; i >= 0; --i) {	//undo the moves in the right order
		Move move = this->turnsHistory[this->currentTurnId][i];
		move.revert();
		assert(this->game.isMovePossible(move));
		
		this->changeTilePosition(move);
		this->game.makeMove(move, true);
	}
	
	this->lastMoveId = -1;
	
// 	for (Player* player: this->players)
// 		player->undoTurn(this->turnsHistory[this->currentTurnId]);
	
	if (this->currentTurnId > 0)
		this->changeCurrentPlayer(true);
	
	emit moveFinished();
}

void GameHandler::redoTurn() {
	qDebug("redoTurn()");
	
	//check if there is a move available in this turn or the next
	if (this->lastMoveId + 1 >= this->turnsHistory[this->currentTurnId].size() && 
		this->currentTurnId + 1 < this->turnsHistory.size()) 
			this->changeCurrentPlayer();
	else if (this->lastMoveId + 1 >= this->turnsHistory[this->currentTurnId].size() && 
		this->currentTurnId + 1 >= this->turnsHistory.size())
			return;
	
	//perform all the moves from the turn
	for (int i = this->lastMoveId + 1; i < this->turnsHistory[this->currentTurnId].size(); ++i) {	//redo the moves in the right order
		Move move = this->turnsHistory[this->currentTurnId][i];
		assert(this->game.isMoveValid(move));
		
		this->changeTilePosition(move);
		this->game.makeMove(move);
	}

	this->lastMoveId = this->turnsHistory[this->currentTurnId].size() - 1;
	
	if (this->currentTurnId + 1 < this->turnsHistory.size())
		this->changeCurrentPlayer();
	
	emit moveFinished();
}