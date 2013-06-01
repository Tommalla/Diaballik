/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include "../DiaballikEngine/src/Singleton.h"
#include "MainWindow.h"
#include <QStateMachine>

/**
 * @brief A Singleton to manage the Qt State Machine
 **/
class StateHandler : public QObject, public Singleton<StateHandler> {
	Q_OBJECT;
	friend class Singleton<StateHandler>;
	private:
		MainWindow* window;
		QStateMachine machine;
		
		QState* GameFinished;
		QState* GameEditor;
		QState* HumanVsHumanGame;
		QState* HumanVsAIGame;
		QState* AIVsAIGame;
		//TODO some boolean container/variables to represent what can be done. States need to change it
		
		StateHandler();
	public:
		void start(MainWindow* window);
		//TODO boolean methods like canMovePawn etc.
};

#endif // STATEHANDLER_H
