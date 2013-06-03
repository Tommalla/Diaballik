/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include <QPainter>
#include <QDebug>
#include "GraphicsTile.h"
#include "GameHandler.h"
#include "SettingsHandler.h"
#include "gameConstants.h"

void GraphicsTile::drawSelection() {
	QPixmap tmp = this->pixmap();
	QPainter painter(&tmp);
	
	this->primarySelectionPen.setColor(this->primarySelectionColor);
	painter.setPen(this->primarySelectionPen);
	
	painter.drawRect(QRect(QPoint(2, 2), QPoint(tmp.width() - 4, tmp.height() - 4)));
	painter.end();
	
	this->setPixmap(tmp);
	
}

void GraphicsTile::mousePressEvent (QGraphicsSceneMouseEvent* event) {
	QGraphicsItem::mousePressEvent (event);
	
	if (this->selected == true) {	//we can only move to a selected tile
		qDebug("Trying to move to (%d, %d) [%d]", this->x, this->y, this->zValue());
		const GraphicsMovableTile* from = GameHandler::getInstance().getLastSelector();
		assert(from != NULL);
		
		if ((from->getPos()) != (this->getPos()))	//if we're not moving to self
			emit makeMove(Move(from->getPos(), this->getPos()));
		else qDebug("Tried to move to self:(");
	}
}


GraphicsTile::GraphicsTile(const QString& graphicsPath, const int x, const int y, const int z, const int width, const int height) {
	this->setZValue(z);
	this->selected = false;
	this->x = x;
	this->y = y;

	this->setAcceptedMouseButtons(Qt::LeftButton);

	
	this->primarySelectionColor = SettingsHandler::getInstance().value("selection/primarySelectionColor", DEFAULT_PRIMARY_SELECTION_COLOR).value<QColor>();
	this->primarySelectionPen = SettingsHandler::getInstance().value("selection/primarySelectionPen", DEFAULT_PRIMARY_SELECTION_PEN).value<QPen>();
	this->originalPixmap = QPixmap(graphicsPath);
	this->redraw(width, height);
}

void GraphicsTile::redraw (int width, int height) {
	if (width == -1 || height == -1) {
		width = this->pixmap().width();
		height = this->pixmap().height();
	}
	
	QPixmap tmp = this->originalPixmap;
	this->setPixmap(tmp.scaled(width, height, Qt::KeepAspectRatio));
	this->setPos(this->x * width, this->y * height);
	
	if (this->selected)
		this->drawSelection();
}

void GraphicsTile::select() {
	qDebug("Tile(%d, %d) getting selected!\n", x, y);
	this->selected = true;
	this->redraw();
}

void GraphicsTile::deselect() {
	this->selected = false;
	this->redraw();
}

const Point GraphicsTile::getPos() const {
	return Point(this->x, this->y);
}


