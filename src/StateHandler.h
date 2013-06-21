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

		vector<QObject*> hideableUI;
		vector<tuple<QObject*, QString, QVariant> > newProperties[4];
		void assignProperties(const ApplicationState& state);
		
		//TODO some boolean container/variables to represent what can be done. States need to change it
		bool gamePaused;
		bool editorMode;
		
		StateHandler();
	public:
		void start(MainWindow* window);
		//TODO boolean methods like canMovePawn etc.
		const bool isGamePaused() const;
		void setGamePaused(const bool val);
		
		const bool isEditorMode() const;
		void disableUI();
		
	public slots:
		void playerChanged();
		void gameFinished();
		void moveFinished();
		void startEditorMode();
		//void finishEditorMode();
	
};

#endif // STATEHANDLER_H
