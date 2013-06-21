/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "GraphicsScene.h"
#include <QKeyEvent>

void GraphicsScene::keyPressEvent (QKeyEvent* event) {
	QGraphicsScene::keyPressEvent (event);
	emit keyPressed(event->key());
	event->accept();
}
