/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef GRAPHICSMOVABLETILE_H
#define GRAPHICSMOVABLETILE_H

#include "GraphicsTile.h"


class GraphicsMovableTile : public GraphicsTile {
	private:
		//TODO qpropertyanimation and stuff like that
	public:
		GraphicsMovableTile (const QString& graphicsPath, const int x, const int y, const int width, const int height);
		/**
		 * @brief Moves the tile to a new position (instantly)
		 **/
		void changePosition(const int newX, const int newY);
		/**
		 * @brief Moves the tile to a new position (smoothly and according to the game rules)
		 **/
		void move(const int newX, const int newY);
};

#endif // GRAPHICSMOVABLETILE_H
