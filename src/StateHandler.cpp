/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "StateHandler.h"

StateHandler::StateHandler() : QObject() {
	//TODO create states here
	this->GameEditor = new QState();
	this->GameFinished = new QState();
	this->HumanVsAIGame = new QState();
	this->HumanVsHumanGame = new QState();
	this->AIVsAIGame = new QState();
	
	//TODO add transitions etc.
	//don't want to look at it right now...
}

void StateHandler::start(MainWindow* window) {
	this->window = window;
	this->machine.start();
}
