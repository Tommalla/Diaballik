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
		~MainWindow();
    
	private:
		Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
