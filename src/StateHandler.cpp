/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include "StateHandler.h"
#include "GameHandler.h"
#include "ui_MainWindow.h"

//this shall be a better implementation...

void StateHandler::assignProperties (const ApplicationState& state) {
	QObject* obj;
	QString name;
	QVariant value;
	for (auto t: this->newProperties[state]) {
		tie(obj, name, value) = t;
		obj->setProperty(qPrintable(name), value);
	}
	
	this->window->ui->previousTurnPushButton->setEnabled(GameHandler::getInstance().canUndo());
	this->window->ui->nextTurnPushButton->setEnabled(GameHandler::getInstance().canRedo());
	this->window->ui->undoPushButton->setEnabled(GameHandler::getInstance().canUndo());
	this->window->ui->redoPushButton->setEnabled(GameHandler::getInstance().canRedo());
}


StateHandler::StateHandler() : QObject() {
	this->gamePaused = false;
}

void StateHandler::start(MainWindow* window) {
	this->window = window;
	
	vector<QObject*> hideableUI = {
		window->ui->currentTurnGroupBox, 
		window->ui->turnsGroupBox,
		window->ui->previousTurnPushButton,
		window->ui->pausePushButton,
		window->ui->nextTurnPushButton,
		window->ui->statusLayout, 
		window->ui->startPushButton, 
		window->ui->stopButton,
		window->ui->botOptionsGroupBox
	};
	
	QVariant values[4][9] = {
		{false, false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, true, false, false},
		{true, true, true, false, true, true, false, false, false},
		{false, true, false, true, false, true, false, true, true}
	};
	
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 9; ++j)
			this->newProperties[i].push_back(make_tuple(hideableUI[j], "enabled", values[i][j].toBool()));
		
	this->gameFinished();
}

const bool StateHandler::isGamePaused() const {
	return gamePaused;
}

void StateHandler::setGamePaused (const bool val) {
	this->gamePaused = val;
	
	this->window->ui->pausePushButton->setText(val ? "Resume" : "Pause");
	this->window->ui->previousTurnPushButton->setEnabled(GameHandler::getInstance().canUndo());
	this->window->ui->nextTurnPushButton->setEnabled(GameHandler::getInstance().canRedo());
}

void StateHandler::gameFinished() {
	this->assignProperties(GAME_FINISHED);
}

void StateHandler::playerChanged() {
	ApplicationPlayerType player = GameHandler::getInstance().getPlayerType();
	this->assignProperties((ApplicationState)player);
	
	//TODO add canUndoTurn
	
	if (player == AI_PLAYER)
		this->window->ui->pausePushButton->setEnabled(GameHandler::getInstance().getPlayerInfo().GTPE);
	else if (player == HUMAN_PLAYER && 
		GameHandler::getInstance().getPlayerType(false) == AI_PLAYER) {
		this->window->ui->turnsGroupBox->setEnabled(GameHandler::getInstance().getPlayerInfo(false).GTPE);
	}
}

void StateHandler::moveFinished() {
	this->window->ui->previousTurnPushButton->setEnabled(GameHandler::getInstance().canUndo());
	this->window->ui->nextTurnPushButton->setEnabled(GameHandler::getInstance().canRedo());
	this->window->ui->undoPushButton->setEnabled(GameHandler::getInstance().canUndo());
	this->window->ui->redoPushButton->setEnabled(GameHandler::getInstance().canRedo());
}



