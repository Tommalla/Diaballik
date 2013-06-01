#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SettingsHandler.h"

namespace Ui {
	class MainWindow;
}

/**
 * @brief The main window of the application
 **/
class MainWindow : public QMainWindow {
	Q_OBJECT;
    
	friend class StateHandler;	//so that states can change the ui
	public:
		explicit MainWindow(QWidget *parent = 0);
		virtual void resizeEvent (QResizeEvent* event);
		~MainWindow();
    
	private:
		Ui::MainWindow *ui;
	private slots:
		void newGame();
		void startGameEditor();
		void showRules();
		void showAuthor();
		void showHelp();
};

#endif // MAINWINDOW_H
