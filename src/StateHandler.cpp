/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include <qeventtransition.h>
#include <qsignaltransition.h>
#include "StateHandler.h"
#include "ui_MainWindow.h"

//not the best implementation ever, but it does the job

StateHandler::StateHandler() : QObject() {
	this->GameFinished = new QState();
	this->HumanVsAIGame = new QState();
	this->HumanVsHumanGame = new QState();
	this->AIVsAIGame = new QState();
	this->BoardEditor = new QState();
	this->Game = new QState();
	
	this->machine.addState(this->AIVsAIGame);
	this->machine.addState(this->HumanVsAIGame);
	this->machine.addState(this->HumanVsHumanGame);
	this->machine.addState(this->GameFinished);
	this->machine.addState(this->BoardEditor);
	this->machine.addState(this->Game);
}

void StateHandler::assignBool (QState* state, vector< QObject* > objects, const char* property, vector< bool > properties) {
	assert (objects.size() == properties.size());
	
	for (int i = 0; i < objects.size(); ++i)
		state->assignProperty(objects[i], property, QVariant(properties[i]));
}


void StateHandler::newGame (const PlayerInfo& playerA, const PlayerInfo& playerB) {
	emit newGameStarted();
	QApplication::processEvents();
	
	if (playerA.type == AI_PLAYER && playerB.type == AI_PLAYER) {	//AI VS AI Game
		emit newAIVsAIGame();
		return;
	}
	
	if (playerA.type == AI_PLAYER || playerB.type == AI_PLAYER) {
		emit newHumanVsAIGame();
		return;
	}
	
	emit newHumanVsHumanGame();
}

void StateHandler::start(MainWindow* window) {
	this->window = window;
	
	this->GameFinished->addTransition(this, SIGNAL(newGameStarted()), this->Game);
	
	this->Game->addTransition(this, SIGNAL(newHumanVsAIGame()), this->HumanVsAIGame);
	this->Game->addTransition(this, SIGNAL(newHumanVsHumanGame()), this->HumanVsHumanGame);
	this->Game->addTransition(this, SIGNAL(newAIVsAIGame()), this->AIVsAIGame);
	
	this->HumanVsHumanGame->addTransition(window->ui->actionBoardEditor, SIGNAL(triggered()), this->BoardEditor);
	this->HumanVsHumanGame->addTransition(&(window->newGameDialog), SIGNAL(accepted()), this->Game);
	this->HumanVsAIGame->addTransition(window->ui->actionBoardEditor, SIGNAL(triggered()), this->BoardEditor);
	this->HumanVsAIGame->addTransition(&(window->newGameDialog), SIGNAL(accepted()), this->Game);
	this->AIVsAIGame->addTransition(window->ui->actionBoardEditor, SIGNAL(triggered()), this->BoardEditor);
	this->AIVsAIGame->addTransition(&(window->newGameDialog), SIGNAL(accepted()), this->Game);
	
	
 	this->BoardEditor->addTransition(window->ui->startPushButton, SIGNAL(clicked()), this->GameFinished);
	//^ because validation comes next and then the new game might eventually be started
	
	vector<QObject*> hideableUI = {window->ui->currentTurnGroupBox, window->ui->turnsGroupBox, 
	window->ui->statusLayout, window->ui->startPushButton, window->ui->stopButton};
	
	this->assignBool(this->BoardEditor, hideableUI, "visible",
		{false, false, false, true, false});
	
	this->assignBool(this->GameFinished, hideableUI, "visible",
		{false, false, false, false, false});
	
	this->assignBool(this->Game, hideableUI, "visible",
		{true, true, true, false, true});
	
	this->HumanVsHumanGame->assignProperty(window->ui->stopButton, "visible", false);

 	this->machine.setInitialState(this->GameFinished);
	this->machine.start();
}
