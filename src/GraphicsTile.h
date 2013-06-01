/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef GRAPHICSTILE_H
#define GRAPHICSTILE_H

#include <QGraphicsItem>


/**
 * @brief A general graphics tile to be drawn on the scene
 * Does not suppor moving, yet it supports input and drawing
 **/
class GraphicsTile : public QGraphicsPixmapItem {
	private:
		int x, y;
	public:
		GraphicsTile(const QString& graphicsPath, const int x, const int y);
		/**
		 * @brief Redraws (scales) internal QPixmap with new width/height
		 **/
		void redraw(const int width, const int height);
		/**
		 * @brief Moves the tile to a new position (instantly)
		 **/
		void changePosition(const int newX, const int newY);
		/**
		 * @brief Moves the tile to a new position (smoothly and according to the game rules)
		 **/
		void move(const int newX, const int newY);
};

#endif // GRAPHICSTILE_H
