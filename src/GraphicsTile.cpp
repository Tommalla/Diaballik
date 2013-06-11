/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include <QPainter>
#include <QDebug>
#include "GraphicsTile.h"
#include "GameHandler.h"
#include "SettingsHandler.h"
#include "gameConstants.h"

void GraphicsTile::drawSelection (const QColor& color) {
	QPixmap tmp = this->pixmap();
	QPainter painter(&tmp);
	
	this->primarySelectionPen.setColor(color);
	painter.setPen(this->primarySelectionPen);
	
	painter.drawRect(QRect(QPoint(2, 2), QPoint(tmp.width() - 4, tmp.height() - 4)));
	painter.end();
	
	this->setPixmap(tmp);
}


void GraphicsTile::drawPrimarySelection() {
	this->drawSelection(this->primarySelectionColor);
}

void GraphicsTile::drawSecondarySelection() {
	this->drawSelection(this->secondarySelectionColor);
}

void GraphicsTile::mousePressEvent (QGraphicsSceneMouseEvent* event) {
	QGraphicsItem::mousePressEvent (event);
	
	if (this->primarySelected == true) {	//we can only move to a selected tile
		qDebug("Trying to move to (%d, %d) [%d]", this->innerX, this->innerY, this->zValue());
		const GraphicsMovableTile* from = GameHandler::getInstance().getLastSelector();
		assert(from != NULL);
		
		if ((from->getPos()) != (this->getPos()))	//if we're not moving to self
				emit makeMove(Move(from->getPos(), this->getPos()));
		
	}
}


GraphicsTile::GraphicsTile(const QString& graphicsPath, const int x, const int y, const int z, const int width, const int height) {
	this->setZValue(z);
	this->primarySelected = this->secondarySelected = false;
	this->innerX = x;
	this->innerY = y;

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
	this->setPos(this->innerX * width, this->innerY * height);
	
	if (this->primarySelected)
		this->drawPrimarySelection();
	if (this->secondarySelected)
		this->drawSecondarySelection();
}

void GraphicsTile::select(const bool primary) {
	qDebug("Tile(%d, %d) getting selected!\n", innerX, innerY);
	this->primarySelected = primary;
	this->secondarySelected = !primary;
	this->redraw();
}

void GraphicsTile::deselect() {
	this->primarySelected = this->secondarySelected = false;
	this->redraw();
}

const Point GraphicsTile::getPos() const {
	return Point(this->innerX, this->innerY);
}


