/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include <qeventtransition.h>
#include <qsignaltransition.h>
#include "StateHandler.h"
#include "GameHandler.h"
#include "ui_MainWindow.h"

//this shall be a better implementation...

void StateHandler::assignProperties (const ApplicationState& state) {
	//TODO assign properties to the objects
}


StateHandler::StateHandler() : QObject() {
	//TODO create newProperties
// 	vector<QObject*> hideableUI = {window->ui->currentTurnGroupBox, window->ui->turnsGroupBox, 
// 	window->ui->statusLayout, window->ui->startPushButton, window->ui->stopButton};
// 	
// 	this->assignBool(this->BoardEditor, hideableUI, "visible",
// 			 {false, false, false, true, false});
// 	
// 	this->assignBool(this->GameFinished, hideableUI, "visible",
// 			 {false, false, false, false, false});
// 	
// 	this->assignBool(this->Game, hideableUI, "visible",
// 			 {true, true, true, false, true});
// 	
// 	this->HumanVsHumanGame->assignProperty(window->ui->stopButton, "visible", false);
// 	this->HumanVsHumanGame->assignProperty(window->ui->pausePushButton, "visible", false);
// 	this->HumanVsAIGame->assignProperty(window->ui->pausePushButton, "visible", false);
}

void StateHandler::newGame (const PlayerInfo& playerA, const PlayerInfo& playerB) {
	//TODO/FIXME could I make it a slot? Well, kinda, think so, yes?
}

void StateHandler::start(MainWindow* window) {
	this->window = window;
	//anything else?
}

void StateHandler::gameFinished() {
	this->assignProperties(GAME_FINISHED);
}

void StateHandler::playerChanged() {
	//TODO get playerInfo from GameHandler and run assignProperties
	this->assignProperties(HUMAN_MOVE);
}


