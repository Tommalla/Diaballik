#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SettingsHandler.h"
#include "NewGameDialog.h"

namespace Ui {
	class MainWindow;
}

/**
 * @brief The main window of the application
 **/
class MainWindow : public QMainWindow {
	Q_OBJECT;
    
	friend class StateHandler;	//so that states can change the ui
	
	private:
		Ui::MainWindow *ui;
		NewGameDialog newGameDialog;
		const qreal getSceneDimension() const;
		PlayerInfo lastPlayerA, lastPlayerB;
		bool loading;
	
	public:
		explicit MainWindow(QWidget *parent = 0);
		virtual void resizeEvent (QResizeEvent* event);
		~MainWindow();
    
	private slots:
		void pauseGame();
		void newGame();
		void loadGame();
		void saveGame();
		void playerChanged();
		void moveFinished();
		void gameFinished();
		void startGameEditor();
		void showRules();
		void showAuthor();
		void showHelp();
		void showSettings();
		void displayError(const QString msg);
};

#endif // MAINWINDOW_H
