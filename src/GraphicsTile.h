/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef GRAPHICSTILE_H
#define GRAPHICSTILE_H

#include <QGraphicsItem>
#include "../DiaballikEngine/src/Point.h"


/**
 * @brief A general graphics tile to be drawn on the scene
 * Does not suppor moving, yet it supports input and drawing
 **/
class GraphicsTile : public QGraphicsPixmapItem {
	private:
		int x, y;
		bool selected;
		QPixmap originalPixmap;
	public:
		GraphicsTile(const QString& graphicsPath, const int x, const int y, const int width, const int height);
		/**
		 * @brief Redraws (scales) internal QPixmap with new width/height
		 **/
		void redraw(const int width, const int height);
		/**
		 * @brief Selects the Tile (visually changes it)
		 **/
		void select();
		/**
		 * @brief Deselects the tile
		 **/
		void deselect();
		
		const Point getPos();
};

#endif // GRAPHICSTILE_H
