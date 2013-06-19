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
	
	this->editorMode = (state == BOARD_EDITOR);
}


StateHandler::StateHandler() : QObject() {
	this->gamePaused = false;
}

void StateHandler::start(MainWindow* window) {
	this->window = window;
	
	vector<QObject*> hideableUI = {
		window->ui->currentTurnGroupBox,
		window->ui->turnDonePushButton,
		window->ui->undoPushButton,
		window->ui->redoPushButton,
		window->ui->turnsGroupBox,
		window->ui->previousTurnPushButton,
		window->ui->pausePushButton,
		window->ui->nextTurnPushButton,
		window->ui->statusLayout, 
		window->ui->startPushButton, 
		window->ui->botOptionsGroupBox,
		window->ui->showHintPushButton
	};
	
	QVariant values[4][12] = {
		{false, false, false, false, false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false, false, true, false, false},
		{true, true, true, true, true, true, false, true, true, false, false, true},
		{false, false, true, true, true, false, true, false, true, false, true, false}
	};
	
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 12; ++j)
			this->newProperties[i].push_back(make_tuple(hideableUI[j], "enabled", values[i][j].toBool()));
		
	this->gameFinished();
}

const bool StateHandler::isGamePaused() const {
	return gamePaused;
}

void StateHandler::setGamePaused (const bool val) {
	if (val == false)
		GameHandler::getInstance().resumeGame();
	
	this->window->ui->pausePushButton->setText(val ? "Resume" : "Pause");
	this->window->ui->previousTurnPushButton->setEnabled(GameHandler::getInstance().canUndo());
	this->window->ui->nextTurnPushButton->setEnabled(GameHandler::getInstance().canRedo());
	
	ApplicationPlayerType player = GameHandler::getInstance().getPlayerType();
	if (player == AI_PLAYER)
		this->window->ui->currentTurnGroupBox->setEnabled(val && 
		GameHandler::getInstance().getPlayerInfo().GTPE);
	
	this->gamePaused = val;
	
	this->window->playerChanged();
	
}

const bool StateHandler::isEditorMode() const {
	return editorMode;
}

void StateHandler::gameFinished() {
	this->assignProperties(GAME_FINISHED);
}

void StateHandler::playerChanged() {
	ApplicationPlayerType player = GameHandler::getInstance().getPlayerType();
	this->assignProperties((ApplicationState)player);
	
	if (player == AI_PLAYER) {
		this->window->ui->pausePushButton->setEnabled(GameHandler::getInstance().getPlayerInfo().GTPE);
		this->window->ui->currentTurnGroupBox->setEnabled(this->gamePaused &&
			GameHandler::getInstance().getPlayerInfo().GTPE);
	}
		
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

void StateHandler::startEditorMode() {
	this->assignProperties(BOARD_EDITOR);
}




