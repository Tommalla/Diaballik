#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "gameConstants.h"
#include "GraphicsScene.h"
#include "GameHandler.h"
#include "StateHandler.h"

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
	GraphicsScene* scene = new GraphicsScene();
	this->ui->graphicsView->setScene(scene);
	StateHandler::getInstance().start(this);
	GameHandler::getInstance().Initialize(scene);
	
	//connecting signals
	QObject::connect(ui->actionZakoncz, SIGNAL(triggered()), this, SLOT(close()));
	QObject::connect(ui->actionNowa, SIGNAL(triggered()), this, SLOT(newGame()));
	QObject::connect(ui->actionEdytorPlanszy, SIGNAL(triggered()), this, SLOT(startGameEditor()));
	QObject::connect(ui->actionKlawiszologia, SIGNAL(triggered()), this, SLOT(showHelp()));
	QObject::connect(ui->actionZasadyGry, SIGNAL(triggered()), this, SLOT(showRules()));
}

MainWindow::~MainWindow() {
	SettingsHandler::getInstance().setValue("windowGeometry/width", this->geometry().width());
	SettingsHandler::getInstance().setValue("windowGeometry/height", this->geometry().height());
	SettingsHandler::getInstance().setValue("windowGeometry/positionX", this->geometry().x());
	SettingsHandler::getInstance().setValue("windowGeometry/positionY", this->geometry().y());
	
	delete ui;
}

void MainWindow::newGame() {
	GameHandler::getInstance().newGame(this->ui->graphicsView->viewport()->geometry());
}

void MainWindow::startGameEditor() {
	//TODO start it!
}

void MainWindow::showRules() {
	//TODO
}

void MainWindow::showAuthor() {
	//TODO
}

void MainWindow::showHelp() {
	//TODO
}



