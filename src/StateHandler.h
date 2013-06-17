/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include "../DiaballikEngine/src/Singleton.h"
#include "gameEnums.h"
#include "MainWindow.h"
#include <vector>

using namespace std;

/**
 * @brief A Singleton to manage the Qt State Machine
 **/
class StateHandler : public QObject, public Singleton<StateHandler> {
	Q_OBJECT;
	friend class Singleton<StateHandler>;
	private:
		MainWindow* window;

		vector<tuple<QObject*, QString, QVariant> > newProperties[4];
		void assignProperties(const ApplicationState& state);
		//TODO some boolean container/variables to represent what can be done. States need to change it
		
		StateHandler();
	public:
		void newGame(const PlayerInfo& playerA, const PlayerInfo& playerB);
		void start(MainWindow* window);
		//TODO boolean methods like canMovePawn etc.
		
	public slots:
		void playerChanged();
		void gameFinished();
	
};

#endif // STATEHANDLER_H
