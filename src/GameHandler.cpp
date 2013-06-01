/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "GameHandler.h"

GameHandler::GameHandler() : QObject() {
	this->initialized = false;
}

void GameHandler::Initialize (GraphicsScene* scene) {
	this->scene = scene;
	this->initialized = true;
}

bool GameHandler::moveTile (const GraphicsTile* tile) {

}


void GameHandler::newGame (bool defaultConfig) {

}

