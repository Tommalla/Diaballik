/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef GRAPHICSMOVABLETILE_H
#define GRAPHICSMOVABLETILE_H

#include <QPropertyAnimation>
#include "GraphicsTile.h"


class GraphicsMovableTile : public GraphicsTile {
	Q_OBJECT;
	Q_PROPERTY(QPointF pos
	READ pos
	WRITE setPos);
	private:
		QPropertyAnimation animation;
	protected:
		virtual void mousePressEvent (QGraphicsSceneMouseEvent* event);
	public:
		GraphicsMovableTile (const QString& graphicsPath, const int x, const int y, const int z, const int width, const int height);
		/**
		 * @brief Changes the tile's inner cooridnates without redrawing it there
		 **/
		void changeCoordinates(const int newX, const int newY);
		/**
		 * @brief Moves the tile to a new position (instantly)
		 **/
		void changePosition(const int newX, const int newY);
		/**
		 * @brief Moves the tile to a new position (smoothly and according to the game rules)
		 **/
		void move(const int newX, const int newY);
		void move(const Point& newPos);
};

#endif // GRAPHICSMOVABLETILE_H
