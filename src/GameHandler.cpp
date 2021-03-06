/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include <QDebug>
#include "GameHandler.h"
#include "SettingsHandler.h"
#include "gameConstants.h"
#include "SaveHandler.h"
#include "StateHandler.h"
#include "../DiaballikEngine/src/functions.h"

void GameHandler::dropHistoryTail() {
	while ((int)this->turnsHistory.size() > this->currentTurnId + 1)
		this->turnsHistory.pop_back();
	
	while ((int)this->movesLeft.size() > this->currentTurnId + 1)
		this->movesLeft.pop_back();

	while ((int)this->turnsHistory.back().size() > this->lastMoveId + 1)
		this->turnsHistory.back().pop_back();
}

const int GameHandler::getNextPlayerId() const {
	return (this->currentPlayer + 1) % PLAYERS_QTY;
}

void GameHandler::changeCurrentPlayer(const bool undo) {
	this->deselectTiles();
	
	this->players[this->getNextPlayerId()]->startTurn();
	this->players[this->currentPlayer]->finishTurn();
	
	if (!undo && this->lastMoveId != (int)this->turnsHistory[currentTurnId].size() - 1)
		this->dropHistoryTail();
	
	if (!undo) {
		this->historyHandler.finishTurn();
		//for (int i = 0; i < PLAYERS_QTY; ++i)
			//this->players[i]->play(this->players[this->currentPlayer]->getPlayerInfo().player,
				//this->turnsHistory[this->currentTurnId]);

		if ((int)this->turnsHistory.size() <= this->currentTurnId + 1)
			this->turnsHistory.push_back(vector<Move>());
		if ((int)this->movesLeft.size() <= this->currentTurnId + 1)
			this->movesLeft.push_back(make_pair(2, 1));
		
		this->movesLeft[this->currentTurnId] = make_pair(this->game.getMovesLeft(), this->game.getPassessLeft());
		this->currentTurnId++;
		this->lastMoveId = -1;
	} else {
// 		qDebug("player change for undo");
		bool tmp = true;
		
		if (this->currentTurnId > 0) {
 			if (this->turnsHistory[this->currentTurnId].empty()) {
//  				this->turnsHistory.pop_back();
				tmp = false;
			}
			
			this->movesLeft.pop_back();
			
			this->currentTurnId--;
			
			//TODO stopped here
			//if (tmp)
				//this->sendUndoTurn(this->players[this->currentPlayer]->getPlayerInfo().player,
					//this->currentTurnId + 1);
			
			this->lastMoveId = this->turnsHistory[this->currentTurnId].size() - 1;
		} else {
			this->currentTurnId = 0;
			this->lastMoveId = -1;
		}
	}
	
	this->currentPlayer = this->getNextPlayerId();
	this->game.setCurrentPlayer(engine::getOppositePlayer(this->game.getCurrentPlayer()), 
		this->movesLeft[this->currentTurnId].first, this->movesLeft[this->currentTurnId].second);
	qDebug("Next player! %s", qPrintable(this->players[this->currentPlayer]->getPlayerInfo().name));
	
	if (this->hintAI != NULL)
		this->hintAI->gameChanged(this->game);
	
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

Player* GameHandler::createPlayer (PlayerInfo info, const int id) {
	GamePlayer fieldPlayer = (id == 0) ? GAME_PLAYER_A : GAME_PLAYER_B;
	info.player = fieldPlayer;
	
	switch (info.type) {
		case HUMAN_PLAYER:
		{
			HumanPlayer* res = new HumanPlayer(info);
			
			for (GraphicsTile* tile: this->backgroundTiles)
				QObject::connect(tile, SIGNAL(makeMove(const Move&)), res, SLOT(setMove(const Move&)));
			
			for (GraphicsMovableTile* tile: this->pawns)
				if (engine::getPlayerFor(this->game.getFieldAt(tile->getPos())) == fieldPlayer)
					QObject::connect(tile, SIGNAL(makeMove(const Move&)), res, SLOT(setMove(const Move&)));
				
			return res;
			break;
		}
		case AI_PLAYER:
		{
			AIPlayer* res = new AIPlayer(info);
			QObject::connect(res, SIGNAL(crashed()), this, SLOT(botCrashed()));
			return res;
			break;
		}
		default:
			return NULL;
	}
	
	return NULL;
}

bool GameHandler::initializePlayers (const PlayerInfo& playerA, const PlayerInfo& playerB) {
	this->deletePlayers();
	
	this->players[0] = this->createPlayer(playerA, 0);
	this->players[1] = this->createPlayer(playerB, 1);
	
	if (this->players[0] == NULL || this->players[1] == NULL)
		return false;
	
	for (int i = 0; i < 2; ++i) {
		QObject::connect(&historyHandler, SIGNAL(turnDone(GamePlayer,vector<Move>)), 
				 players[i], SLOT(play(GamePlayer,vector<Move>)));
		QObject::connect(&historyHandler, SIGNAL(turnUndone(GamePlayer,vector<Move>)), 
				 players[i], SLOT(undoTurn(GamePlayer,vector<Move>)));
	}
	
	return true;
}

void GameHandler::sendUndoTurn (const GamePlayer& player, const int turnId) {
	if (turnId == (int)this->turnsHistory.size() - 1 ||
		this->turnsHistory[turnId].empty() == true )	//the turn hasn't been sent to the bot yet
		return;
	
	for (Player* p: this->players)
		if (p != NULL)
			p->undoTurn(player, this->turnsHistory[turnId]);
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
	GraphicsMovableTile* src = NULL;
	
	if (field == BALL_A || field == BALL_B)
		src = this->getBallAt(move.from);
	else
		src = this->getPawnAt(move.from);
	
	//assert(src != NULL);
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

GameHandler::GameHandler() : QObject(), historyHandler(GAME_PLAYER_A) {
	this->initialized = false;
	this->lastSelector = NULL;
	this->players[0] = this->players[1] = NULL;
	this->hintAI = NULL;
	
	this->playersTimer.setInterval(
		SettingsHandler::getInstance().value("application/playersTimerInterval", DEFAULT_PLAYERS_TIMER_INTERVAL).toInt() );
	this->playersTimer.stop();
	
	QObject::connect(&(this->playersTimer), SIGNAL(timeout()), this, SLOT(checkForNewMoves()));
}

GameHandler::~GameHandler() {
	this->deletePlayers();
	
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


void GameHandler::moveTile (const Move& move, const bool animate) {
	GraphicsMovableTile* src = this->getSource(move);
	this->deselectTiles();
	if (animate == true)
		src->move(move.to);
	else
		src->changePosition(move.to.x, move.to.y);
}

void GameHandler::showDestinationsFor (GraphicsMovableTile* tile) {
	this->deselectTiles();
	FieldState field = this->game.getFieldAt(tile->getPos());
	
	if (StateHandler::getInstance().isEditorMode() == true) {	//we're in editor mode 
		if (field == PLAYER_A || field == PLAYER_B) {
			for (GraphicsTile* dstTile: this->backgroundTiles)
				if ( this->game.getFieldAt(dstTile->getPos()) == EMPTY) {
					dstTile->select();
					this->selectedTiles.push_back(dstTile);
				}
		} else {
			for (GraphicsTile* dstTile: this->pawns)
				if (this->game.getFieldAt(dstTile->getPos()) == 
					((field == BALL_A) ? PLAYER_A : PLAYER_B)) {
					dstTile->select();
					this->selectedTiles.push_back(dstTile);
				}
		}
	} else {
		if (this->game.getMovesLeft() <= 0 && this->game.getPassessLeft() <= 0)
			return;
		if ((field == PLAYER_A || field == PLAYER_B) && this->game.getMovesLeft() <= 0)
			return;
		if ((field == BALL_A || field == BALL_B) && this->game.getPassessLeft() <= 0)
			return;
		
		if (this->players[currentPlayer]->getPlayerInfo().type == AI_PLAYER)
			return;
	
		if (engine::getPlayerFor(field) != this->game.getCurrentPlayer())
			return;	//wrong player, we won't be selecting anything
	
	
		vector<Point> destinations = this->game.getDestinationsFor(tile->getPos());
	
		for (Point dst: destinations)
			if (field == PLAYER_A || field == PLAYER_B) {
				for (GraphicsTile* dstTile: this->backgroundTiles)
					if (dst == dstTile->getPos()) {
						dstTile->select();
						this->selectedTiles.push_back(dstTile);
						break;
					}
			} else {
				for (GraphicsTile* dstTile: this->pawns)
					if (dst == dstTile->getPos()) {
						dstTile->select();
						this->selectedTiles.push_back(dstTile);
						break;
					}
			}
	}
		
	this->lastSelector = tile;
	tile->select(false);
	
	if (field == BALL_A || field == BALL_B)	//a hack to get the ball to cancel the selection
		for(GraphicsTile* ball: this->balls)
			if (ball->getPos() == tile->getPos()) {
				ball->select(false);
				this->selectedTiles.push_back(ball);
				break;
			}
}

void GameHandler::displayHint() {
	vector<Move> hint;
	for (int i = 0; i <= this->lastMoveId; ++i)
		hint.push_back(this->turnsHistory[this->currentTurnId][i]);
	
	hint = this->hintAI->generateHint(hint);
	
	if (hint.empty())
		return;
	
	this->deselectTiles();
	
	int i;
	for (i = 0; i <= this->lastMoveId; ++i)
		if (this->turnsHistory[this->currentTurnId][i] != hint[i])
			break;
		
	qDebug("Received hint: %d %d -> %d %d", hint[i].from.x, hint[i].from.y, hint[i].to.x, hint[i].to.y);
		
	GraphicsMovableTile* tile = this->getSource(hint[i]);
	assert(tile != NULL);
	FieldState field = this->game.getFieldAt(tile->getPos());
	
	if (field == PLAYER_A || field == PLAYER_B) {
		for (GraphicsTile* dstTile: this->backgroundTiles)
			if (hint[i].to == dstTile->getPos()) {
				dstTile->select();
				this->selectedTiles.push_back(dstTile);
				break;
			}
	} else {
		for (GraphicsTile* dstTile: this->pawns)
			if (hint[i].to == dstTile->getPos()) {
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

void GameHandler::handleKey (const int key) {
	if (key == Qt::Key_Space) {
		if (StateHandler::getInstance().isGamePaused()) {
			this->resumeGame();
			StateHandler::getInstance().setGamePaused(false);
		} else
			if (this->currentPlayer >= 0 && this->currentPlayer < PLAYERS_QTY && this->players[this->currentPlayer] != NULL &&
				this->players[this->currentPlayer]->getPlayerInfo().type == AI_PLAYER)
				StateHandler::getInstance().setGamePaused(true);
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

bool GameHandler::newGame (const PlayerInfo& playerA, const PlayerInfo& playerB,
			   const int tileSize, bool defaultConfig) {
	this->lastSelector = NULL;
	this->hashes.clear();
	this->hashesHistory.clear();
	vector<Point> pawns[2];
	vector<Point> balls;
	this->game = Game(GAME_PLAYER_A);
	
	if (defaultConfig == true) {	//default board
		balls = {Point(3, 0), Point(3, 6)};
		
		for (int i = 0; i < 2; ++i)
			for (int x = 0; x < 7; ++x)
				pawns[i].push_back(Point(x, 6 * i));
	} else {	//read the board from ui
		if (this->pawns.empty() || this->balls.empty())
			return false;
		
		int tmp = (int)this->pawns.size() / 2;
		for (int i = 0; i < 2; ++i)
			for (int j = i * tmp; j < (i + 1) * tmp; ++j)
				pawns[i].push_back(this->pawns[j]->getPos());
			
		for (GraphicsTile* tile: this->balls)
			balls.push_back(tile->getPos());
		
		this->game.newGame(pawns[0], pawns[1], balls);
		this->game.setCurrentPlayer(GAME_PLAYER_A);
	}
	
	this->createSceneBoard(tileSize, pawns, balls);
	
	this->currentPlayer = 0;
	
	if (this->initializePlayers(playerA, playerB) == false)
		return false;
	
	if (!defaultConfig) {
		for (Player* player: this->players)
			player->newGame(pawns[0], pawns[1], balls, this->game.getCurrentPlayer());
	}
	
	this->players[this->getNextPlayerId()]->finishTurn();
	this->players[this->currentPlayer]->startTurn();
	
	this->lastMoveId = -1;
	this->currentTurnId = 0;
	this->turnsHistory = {vector<Move>()};
	this->movesLeft = {{2, 1}};
	this->hashes.insert(QString::fromStdString(this->game.getHash()));
	
	if (playerA.type == HUMAN_PLAYER || playerB.type == HUMAN_PLAYER) {
		delete this->hintAI;
		this->hintAI = new HintAI(pawns[0], pawns[1], balls, this->game.getCurrentPlayer());
	}
	
	this->playersTimer.start();
	return true;
}

bool GameHandler::loadGame (const QString filename, const PlayerInfo& playerA, const PlayerInfo& playerB, 
			    const int tileSize) {
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
	vector<Point> black = tmpGame.getPawnsOf(GAME_PLAYER_A);
	vector<Point> white = tmpGame.getPawnsOf(GAME_PLAYER_B);
	vector<Point> origBalls;
	for (Point pawn: black)
		if (tmpGame.getFieldAt(pawn) == BALL_A) {
			origBalls.push_back(pawn);
			break;
		}
		
	for (Point pawn: white)
		if (tmpGame.getFieldAt(pawn) == BALL_B) {
			origBalls.push_back(pawn);
			break;
		}
			
	
	//board validated, history created, time to draw!
	this->createSceneBoard(tileSize, pawns, balls);
	this->initializePlayers(playerA, playerB);
	
	for (Player* p: this->players)
		p->newGame(black, white, origBalls, engine::getOppositePlayer(tmpGame.getCurrentPlayer()));
	
	this->movesLeft.clear();
	this->hashes.clear();
	this->hashesHistory.clear();
	
	this->hashes.insert(QString::fromStdString(tmpGame.getHash()));
	
	for (vector<Move> moves: this->turnsHistory) {
		for (Move move: moves) {
			qDebug("%d %d -> %d %d", move.from.x, move.from.y, move.to.x, move.to.y);
			if (!tmpGame.isMoveValid(move))
				return false;
			tmpGame.makeMove(move);
		}
	
		for (Player* p: this->players)
			p->play(tmpGame.getCurrentPlayer(), moves);
		
		movesLeft.push_back(make_pair(tmpGame.getMovesLeft(), tmpGame.getPassessLeft()));
	
		p = (p + 1) % 2;
		tmpGame.setCurrentPlayer((p == 0) ? GAME_PLAYER_A : GAME_PLAYER_B);
		QString h = QString::fromStdString(tmpGame.getHash());
		
		if (hashes.contains(h))
			return false;
		
		hashes.insert(h);
		hashesHistory.push_back(h);
	}
	
	tmpGame.setCurrentPlayer((p == 0) ? GAME_PLAYER_A : GAME_PLAYER_B, movesLeft.back().first, movesLeft.back().second);
	
	if (!this->turnsHistory.empty()) {
		this->currentTurnId = max(0, (int)this->turnsHistory.size() - 1);
		this->lastMoveId = this->turnsHistory.back().size() - 1;
	} else {
		this->currentTurnId = 0;
		this->lastMoveId = -1;
	}
	
	this->game = tmpGame;
	
	StateHandler::getInstance().setGamePaused(true);
	this->players[this->currentPlayer]->startTurn();
	this->players[this->getNextPlayerId()]->finishTurn();
	this->playersTimer.start();
	
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
	while ((int)history.size() > this->currentTurnId + 1)
		history.pop_back();
	while ((int)history.back().size() > this->lastMoveId + 1)
		history.back().pop_back();
	
	if (!save.save(figures, this->currentPlayer, this->turnsHistory))
		return false;
	
	return true;
}


const QString GameHandler::getPlayerName (const bool current) const {
	if (this->players[0] == NULL || this->players[1] == NULL)
		return "";
	return this->players[((current) ? this->currentPlayer : this->getNextPlayerId())]->getPlayerInfo().name;
}

const QString GameHandler::getWinnerName() const {
	if (this->game.isFinished() == false || this->game.getWinner() == NONE)
		return "";
	return this->getPlayerName();
}

const ApplicationPlayerType GameHandler::getPlayerType (const bool current) const {
	if (this->players[0] == NULL || this->players[1] == NULL)
		return NO_PLAYER;
	return this->players[((current) ? this->currentPlayer : this->getNextPlayerId())]->getPlayerInfo().type;
}

const PlayerInfo& GameHandler::getPlayerInfo (const bool current) const {
	return this->players[((current) ? this->currentPlayer : this->getNextPlayerId())]->getPlayerInfo();
}

const bool GameHandler::canUndo() const {
	return this->lastMoveId > -1 || this->currentTurnId > 0;
}

const bool GameHandler::canRedo() const {
	if (this->turnsHistory.empty())
		return false;
	return this->lastMoveId + 1 < (int)this->turnsHistory[this->currentTurnId].size() ||
		(this->currentTurnId + 1 < (int)this->turnsHistory.size() && !this->turnsHistory[this->currentTurnId + 1].empty());
}

void GameHandler::resumeGame() {
	if (StateHandler::getInstance().isGamePaused() == false)
		return;
	
	//we have to undo the current turn so that the AI would start with no moves
 	if (this->players[this->currentPlayer]->getPlayerInfo().type == AI_PLAYER) {
		if (this->lastMoveId > -1)
			this->undoTurn();
// 		else if (this->currentTurnId < (int)this->turnsHistory.size() &&  this->turnsHistory[this->currentTurnId].empty() == false)
// 			this->sendUndoTurn(this->game.getCurrentPlayer(), this->currentTurnId);
	}
}

const int GameHandler::getMovesLeft() const {
	return this->game.getMovesLeft();
}

const int GameHandler::getPassesLeft() const {
	return this->game.getPassessLeft();
}

void GameHandler::checkForNewMoves() {
	if (StateHandler::getInstance().isEditorMode()) {
		for (Player* player: this->players)
			if (player->isMoveReady()) {
				Move move = player->getMove();
				if (this->getSource(move) != NULL) {
					this->moveTile(move, false);
					this->game.makeUnsafeMove(move);
				}
			}
		return;
	}
	
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
			if (this->lastMoveId + 1 < (int)this->turnsHistory[this->currentTurnId].size() &&
				this->turnsHistory[this->currentTurnId][this->lastMoveId + 1] != move) {	//a different move
				this->sendUndoTurn(this->players[this->currentPlayer]->getPlayerInfo().player, this->currentTurnId);
				this->dropHistoryTail();
			}
			
			this->lastMoveId++;
			if ((int)this->turnsHistory[this->currentTurnId].size() == this->lastMoveId)
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

//  	this->players[this->getNextPlayerId()]->startTurn();
// 	this->players[this->currentPlayer]->finishTurn();
	this->changeCurrentPlayer();
	this->resumeGame();
	StateHandler::getInstance().setGamePaused(false);
}

void GameHandler::undoMove() {
	qDebug("\nundoMove() %d %d", this->currentTurnId, this->lastMoveId);
	StateHandler::getInstance().setGamePaused(true);

	if (this->lastMoveId < 0 && this->currentTurnId > 0) {
		this->changeCurrentPlayer(true);	//we cannot move back, have to switch player
	} else if (this->lastMoveId == 0 /*&& this->currentTurnId == 0*/) {	//it's the last move we can undo from this turn
		if (this->turnsHistory[this->currentTurnId].empty() == false)
			this->sendUndoTurn(this->players[this->currentPlayer]->getPlayerInfo().player, this->currentTurnId);
// 			for (Player* player: this->players)
// 				player->undoTurn(this->players[this->currentPlayer]->getPlayerInfo().player,
// 						 this->turnsHistory[this->currentTurnId]);
// 		return;
	}
	
	Move move = this->turnsHistory[this->currentTurnId][this->lastMoveId];
	move.revert();
	
	if (this->game.isMovePossible(move)) {
		qDebug("Undo possible!");
		this->moveTile(move);
		this->lastMoveId--;
		this->game.makeMove(move, true);
		emit moveFinished();
	}
}

void GameHandler::redoMove() {
	qDebug("\nredoMove() %d %d", this->currentTurnId, this->lastMoveId);
	//move to the current turn
	if (this->lastMoveId + 1 >= (int)this->turnsHistory[this->currentTurnId].size() && 
		this->currentTurnId + 1 < (int)this->turnsHistory.size())
		this->changeCurrentPlayer();
	else if (this->lastMoveId + 1 >= (int)this->turnsHistory[this->currentTurnId].size() && 
		this->currentTurnId + 1 >= (int)this->turnsHistory.size())
		return;
	
	//send the move to the current player
	this->players[this->currentPlayer]->setMove(this->turnsHistory[this->currentTurnId][this->lastMoveId + 1]);
}

void GameHandler::undoTurn() {
	qDebug("\nundoTurn()");
	
	StateHandler::getInstance().setGamePaused(true);
	
	//if we need to change the player
	if (this->currentTurnId > 0 && this->lastMoveId == -1)
		this->changeCurrentPlayer(true);
	else 	//or it's mid-turn and we have to go back to the beginning
		if (this->lastMoveId  > -1)
			this->sendUndoTurn(this->players[this->currentPlayer]->getPlayerInfo().player, this->currentTurnId);
	
	for (int i = this->lastMoveId; i >= 0; --i) {	//undo the moves in the right order
		Move move = this->turnsHistory[this->currentTurnId][i];
		move.revert();
		assert(this->game.isMovePossible(move));
		
		this->moveTile(move, false);
		this->game.makeMove(move, true);
	}
	
	this->lastMoveId = -1;
	
	//if it was the first turn, we need to let the players know it was undone
	if (this->currentTurnId == 0 && this->lastMoveId == -1)
		this->sendUndoTurn(this->players[this->currentPlayer]->getPlayerInfo().player, this->currentTurnId);
	
	emit moveFinished();
}

void GameHandler::redoTurn() {
	qDebug("\nredoTurn()");
	StateHandler::getInstance().setGamePaused(true);
	
	//check if there is a move available in this turn or the next
	if (this->lastMoveId + 1 >= (int)this->turnsHistory[this->currentTurnId].size() && 
		this->currentTurnId + 1 < (int)this->turnsHistory.size()) 
			this->changeCurrentPlayer();
	else if (this->lastMoveId + 1 >= (int)this->turnsHistory[this->currentTurnId].size() && 
		this->currentTurnId + 1 >= (int)this->turnsHistory.size())
			return;
	
	//perform all the moves from the turn
	for (int i = this->lastMoveId + 1; i < (int)this->turnsHistory[this->currentTurnId].size(); ++i) {	//redo the moves in the right order
		Move move = this->turnsHistory[this->currentTurnId][i];
		assert(this->game.isMoveValid(move));
		
		this->moveTile(move, false);
		this->game.makeMove(move);
	}

	this->lastMoveId = this->turnsHistory[this->currentTurnId].size() - 1;
	
	if (this->currentTurnId + 1 < (int)this->turnsHistory.size() || 
		this->players[this->currentPlayer]->getPlayerInfo().type == AI_PLAYER)
		this->changeCurrentPlayer();
	
	emit moveFinished();
}

void GameHandler::finishGame() {
	for (Player* player: this->players)
		if (player != NULL)
			player->endGame(false);
}

void GameHandler::botCrashed() {
	emit error("The AI has crashed! :(");
	this->playersTimer.stop();
	emit gameFinished();
}

void GameHandler::startEditor() {
	this->playersTimer.stop();
	this->deletePlayers();
	this->initializePlayers(make_human_player("dummy"), make_human_player("dummy"));
	
	for (Player* player: this->players)
		player->startTurn();
	
	StateHandler::getInstance().startEditorMode();
	this->playersTimer.start();
}

