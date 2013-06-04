#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "gameConstants.h"
#include "GraphicsScene.h"
#include "GameHandler.h"
#include "StateHandler.h"
#include "NewGameDialog.h"

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
	this->ui->graphicsView->setAlignment(Qt::AlignCenter);
	StateHandler::getInstance().start(this);
	GameHandler::getInstance().Initialize(scene);
	
	//connecting signals
	QObject::connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	QObject::connect(ui->actionNew, SIGNAL(triggered()), &(this->newGameDialog), SLOT(exec()));
	QObject::connect(&(this->newGameDialog), SIGNAL(accepted()), this, SLOT(newGame()));
	QObject::connect(ui->actionBoardEditor, SIGNAL(triggered()), this, SLOT(startGameEditor()));
	QObject::connect(ui->actionShortcuts, SIGNAL(triggered()), this, SLOT(showHelp()));
	QObject::connect(ui->actionRules, SIGNAL(triggered()), this, SLOT(showRules()));
}

void MainWindow::resizeEvent (QResizeEvent* event) {
	QWidget::resizeEvent(event);
	GameHandler::getInstance().repaintTiles(this->ui->graphicsView->viewport()->rect());
	this->ui->graphicsView->setSceneRect(0, 0, this->ui->graphicsView->viewport()->width(),
					     this->ui->graphicsView->viewport()->height());
}


MainWindow::~MainWindow() {
	SettingsHandler::getInstance().setValue("windowGeometry/width", this->geometry().width());
	SettingsHandler::getInstance().setValue("windowGeometry/height", this->geometry().height());
	SettingsHandler::getInstance().setValue("windowGeometry/positionX", this->geometry().x());
	SettingsHandler::getInstance().setValue("windowGeometry/positionY", this->geometry().y());
	
	SettingsHandler::getInstance().sync();
	delete ui;
}

void MainWindow::newGame() {
	//TODO add getting players from newGameDialog
	this->ui->graphicsView->setSceneRect(0, 0, this->ui->graphicsView->viewport()->width(),
					     this->ui->graphicsView->viewport()->height());
	PlayerInfo playerA = this->newGameDialog.getPlayerInfo(0);
	PlayerInfo playerB = this->newGameDialog.getPlayerInfo(1);
	
	GameHandler::getInstance().newGame(playerA, playerB, this->ui->graphicsView->viewport()->rect());
	
	StateHandler::getInstance().newGame(playerA, playerB);
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



