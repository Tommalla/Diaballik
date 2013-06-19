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
}


StateHandler::StateHandler() : QObject() {}

void StateHandler::start(MainWindow* window) {
	this->window = window;
	
	vector<QObject*> hideableUI = {
		window->ui->currentTurnGroupBox, 
		window->ui->turnsGroupBox, 
		window->ui->statusLayout, 
		window->ui->startPushButton, 
		window->ui->stopButton
	};
	
	QVariant values[4][5] = {
		{false, false, false, false, false},
		{false, false, false, true, false},
		{true, true, true, false, false},
		{false, true, true, false, true}
	};
	
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 5; ++j)
			this->newProperties[i].push_back(make_tuple(hideableUI[j], "enabled", values[i][j].toBool()));
		// 	this->HumanVsHumanGame->assignProperty(window->ui->stopButton, "visible", false);
		// 	this->HumanVsHumanGame->assignProperty(window->ui->pausePushButton, "visible", false);
		// 	this->HumanVsAIGame->assignProperty(window->ui->pausePushButton, "visible", false);
	//anything else?
		
	this->gameFinished();
}

void StateHandler::gameFinished() {
	this->assignProperties(GAME_FINISHED);
}

void StateHandler::playerChanged() {
	this->assignProperties((ApplicationState)GameHandler::getInstance().getPlayerType());
}


