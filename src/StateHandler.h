/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include "../DiaballikEngine/src/Singleton.h"
#include "MainWindow.h"
#include <QStateMachine>
#include <vector>

using namespace std;

/**
 * @brief A Singleton to manage the Qt State Machine
 **/
class StateHandler : public QObject, public Singleton<StateHandler> {
	Q_OBJECT;
	friend class Singleton<StateHandler>;
	signals:
		void newGameStarted();
		void newHumanVsHumanGame();
		void newHumanVsAIGame();
		void newAIVsAIGame();
	private:
		MainWindow* window;
		QStateMachine machine;
		
		QState* GameFinished;
		QState* Game;
		QState* HumanVsHumanGame;
		QState* HumanVsAIGame;
		QState* AIVsAIGame;
		QState* BoardEditor;
		//TODO some boolean container/variables to represent what can be done. States need to change it
		
		StateHandler();
		void assignBool(QState* state, vector< QObject* > objects, const char* property, vector< bool > properties);
	public:
		void newGame(const PlayerInfo& playerA, const PlayerInfo& playerB);
		void start(MainWindow* window);
		//TODO boolean methods like canMovePawn etc.
};

#endif // STATEHANDLER_H
