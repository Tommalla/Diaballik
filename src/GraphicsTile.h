/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef GRAPHICSTILE_H
#define GRAPHICSTILE_H

#include <QGraphicsPixmapItem>
#include <QPen>
#include "../DiaballikEngine/src/Point.h"
#include "../DiaballikEngine/src/Move.h"


/**
 * @brief A general graphics tile to be drawn on the scene
 * Does not suppor moving, yet it supports input and drawing
 **/
class GraphicsTile : public QObject, public QGraphicsPixmapItem {
	Q_OBJECT;
	private:
		int x, y;
		QPixmap originalPixmap;
		QColor primarySelectionColor;
		QPen primarySelectionPen;
		
		void drawSelection();	//draws selection around the item
	protected:
		bool selected;
		virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
	public:
		GraphicsTile(const QString& graphicsPath, const int x, const int y, const int width, const int height);
		/**
		 * @brief Redraws (scales) internal QPixmap with new width/height
		 * If any of parameters is set to -1, the pixmap is redrawn with its' current size
		 * @param width Default value is -1
		 * @param height Default value is -1
		 **/
		void redraw(int width = -1, int height = -1);
		/**
		 * @brief Selects the Tile (visually changes it)
		 **/
		void select();
		/**
		 * @brief Deselects the tile
		 **/
		void deselect();
		
		const Point getPos() const;
	signals:
		void makeMove(const Move& move);
	
};

#endif // GRAPHICSTILE_H
