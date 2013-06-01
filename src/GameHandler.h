/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <QObject>
#include "../DiaballikEngine/src/Singleton.h"
#include "GraphicsScene.h"


/**
 * @brief The main class of the application
 * Handles everything game-related - from creating and managing everything that is visible on the Scene
 * to saving/loading the game, beginning the new one, initializing players etc.
 * 
 * @warning You cannot use this class without calling Initalize(GraphicsScene*) first!
 **/
class GameHandler : public QObject, public Singleton<GameHandler> {
	Q_OBJECT;
	friend class Singleton<GameHandler>;
	private:
		bool initialized;
		GraphicsScene* scene;
		
		GameHandler();
	public:
		void Initialize(GraphicsScene* scene);
};

#endif // GAMEHANDLER_H
