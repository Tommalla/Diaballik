/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

/**
 * @brief Own implementation of GraphicsScene
 * It adds mouse/keyboard handling and drag'n'drop support for the editor.
 * (Or was supposed to...)
 **/
class GraphicsScene : public QGraphicsScene {
	Q_OBJECT;
	protected:
		virtual void keyPressEvent (QKeyEvent* event);
	signals:
		void keyPressed(const int key);
};

#endif // GRAPHICSSCENE_H
