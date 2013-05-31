#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "constants.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), 
					ui(new Ui::MainWindow) {
	this->ui->setupUi(this);
	
	//appearance settings - title, position, size
	this->setWindowTitle(APP_NAME);
	this->resize(SettingsHandler::getInstance().value("windowGeometry/width", this->geometry().width()).toInt(),
		SettingsHandler::getInstance().value("windowGeometry/height", this->geometry().height()).toInt());
	
	this->move(SettingsHandler::getInstance().value("windowGeometry/positionX", this->geometry().x()).toInt(),
		   SettingsHandler::getInstance().value("windowGeometry/positionY", this->geometry().y()).toInt());
	
	//creating all necessary objects
	//TODO - implement and create 'em
	
	//connecting signals
	QObject::connect(ui->actionZakoncz, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow() {
	SettingsHandler::getInstance().setValue("windowGeometry/width", this->geometry().width());
	SettingsHandler::getInstance().setValue("windowGeometry/height", this->geometry().height());
	SettingsHandler::getInstance().setValue("windowGeometry/positionX", this->geometry().x());
	SettingsHandler::getInstance().setValue("windowGeometry/positionY", this->geometry().y());
	
	delete ui;
}
