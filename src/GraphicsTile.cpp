/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "GraphicsTile.h"

GraphicsTile::GraphicsTile(const QString& graphicsPath, const int x, const int y, const int width, const int height) {
	this->selected = false;
	
	this->originalPixmap = QPixmap(graphicsPath);
	this->redraw(width, height);
	
	this->setPos(x * width, y * height);
}

void GraphicsTile::redraw (const int width, const int height) {
	QPixmap tmp = this->originalPixmap;
	this->setPixmap(tmp.scaled(width, height));
}

void GraphicsTile::select() {
	//TODO implement
}

void GraphicsTile::deselect() {
	//TODO implement
}

const Point GraphicsTile::getPos() {
	return Point(x, y);
}


