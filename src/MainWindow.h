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
	
	public:
		explicit MainWindow(QWidget *parent = 0);
		virtual void resizeEvent (QResizeEvent* event);
		~MainWindow();
    
	private slots:
		void newGame();
		void startGameEditor();
		void showRules();
		void showAuthor();
		void showHelp();
};

#endif // MAINWINDOW_H
