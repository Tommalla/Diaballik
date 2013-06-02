/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "GraphicsMovableTile.h"
#include "GameHandler.h"

void GraphicsMovableTile::mousePressEvent (QGraphicsSceneMouseEvent* event) {
	GraphicsTile::mousePressEvent (event);
	
	if (this->selected == false)	//start a new selection
		GameHandler::getInstance().showDestinationsFor(this);
}


GraphicsMovableTile::GraphicsMovableTile (const QString& graphicsPath, const int x, const int y, const int width, const int height) : 
	GraphicsTile (graphicsPath, x, y, width, height) {

}
