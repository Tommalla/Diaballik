/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "GraphicsMovableTile.h"
#include "GameHandler.h"
#include "SettingsHandler.h"
#include "gameConstants.h"
#include <qgraphicssceneevent.h>

void GraphicsMovableTile::mousePressEvent (QGraphicsSceneMouseEvent* event) {
	GraphicsTile::mousePressEvent (event);
	
	if (this->secondarySelected == true) {
		GameHandler::getInstance().deselectTiles();
		event->accept();
	} else if (this->primarySelected == false)	//start a new selection
		GameHandler::getInstance().showDestinationsFor(this);
}


GraphicsMovableTile::GraphicsMovableTile (const QString& graphicsPath, const int x, const int y, const int z, const int width, const int height) : 
	GraphicsTile (graphicsPath, x, y, z, width, height) {
	
	this->animation.setTargetObject(this);
	this->animation.setPropertyName("pos");
}

void GraphicsMovableTile::changeCoordinates (const int newX, const int newY) {
	this->innerX = newX;
	this->innerY = newY;
}


void GraphicsMovableTile::changePosition (const int newX, const int newY) {
	this->changeCoordinates(newX, newY);
	this->redraw();
}

void GraphicsMovableTile::move (const int newX, const int newY) {
	int durationPerField = SettingsHandler::getInstance().value("animation/duration", DEFAULT_ANIMATION_DURATION).toInt();
	int fields = max( abs(this->innerX - newX), abs(this->innerY - newY));
	
	this->changeCoordinates(newX, newY);
	this->animation.setDuration(durationPerField * (log2(fields) + 1));
	this->animation.setEndValue(QPoint(newX * this->pixmap().width(), newY * this->pixmap().height()));
	this->animation.start();
}

void GraphicsMovableTile::move (const Point& newPos) {
	this->move(newPos.x, newPos.y);
}


