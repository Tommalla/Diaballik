#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "constants.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), 
					ui(new Ui::MainWindow) {
	this->ui->setupUi(this);
	
	//appearance settings - title, position, size
	this->setWindowTitle(APP_NAME);
	//TODO: take window position and size from settings
	
	//creating all necessary objects
	//TODO - implement and create 'em
	
	//connecting signals
	QObject::connect(ui->actionZakoncz, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow() {
	//TODO write necessary settings to settings
	delete ui;
}
