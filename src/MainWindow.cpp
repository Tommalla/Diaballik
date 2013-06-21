#include <cassert>
#include <QFileDialog>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "gameConstants.h"
#include "GraphicsScene.h"
#include "GameHandler.h"
#include "StateHandler.h"
#include "NewGameDialog.h"

const qreal MainWindow::getSceneDimension() const {
	return min(this->ui->graphicsView->viewport()->width(), this->ui->graphicsView->viewport()->height());
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), 
					ui(new Ui::MainWindow) {
	this->ui->setupUi(this);
	this->loading = false;
	
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
	QObject::connect(ui->actionAuthor, SIGNAL(triggered()), this, SLOT(showAuthor()));
	QObject::connect(ui->turnDonePushButton, SIGNAL(clicked()), &(GameHandler::getInstance()), SLOT(currentTurnDone()));
	QObject::connect(&(GameHandler::getInstance()), SIGNAL(playerChanged()), this, SLOT(playerChanged()));
	QObject::connect(&(GameHandler::getInstance()), SIGNAL(gameFinished()), this, SLOT(gameFinished()));
	QObject::connect(ui->undoPushButton, SIGNAL(clicked()), &(GameHandler::getInstance()), SLOT(undoMove()));
	QObject::connect(ui->redoPushButton, SIGNAL(clicked()), &(GameHandler::getInstance()), SLOT(redoMove()));
	QObject::connect(ui->previousTurnPushButton, SIGNAL(clicked()), &(GameHandler::getInstance()), SLOT(undoTurn()));
	QObject::connect(ui->nextTurnPushButton, SIGNAL(clicked()), &(GameHandler::getInstance()), SLOT(redoTurn()));
	QObject::connect(&(GameHandler::getInstance()), SIGNAL(moveFinished()), this, SLOT(moveFinished()));
	QObject::connect(&(GameHandler::getInstance()), SIGNAL(moveFinished()), &(StateHandler::getInstance()), SLOT(moveFinished()));
	QObject::connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(loadGame()));
	QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveGame()));
	QObject::connect(ui->pausePushButton, SIGNAL(clicked()), this, SLOT(pauseGame()));
	QObject::connect(&(GameHandler::getInstance()), SIGNAL(error(QString)), this, SLOT(displayError(QString)));
	QObject::connect(ui->startPushButton, SIGNAL(clicked()), &(this->newGameDialog), SLOT(exec()));
	QObject::connect(ui->showHintPushButton, SIGNAL(clicked()), &(GameHandler::getInstance()), SLOT(displayHint()));
	QObject::connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
}

void MainWindow::resizeEvent (QResizeEvent* event) {
	QWidget::resizeEvent(event);
	qreal dimension = this->getSceneDimension();
	
	GameHandler::getInstance().repaintTiles(dimension / 7.0);
	this->ui->graphicsView->setSceneRect(0, 0, dimension, dimension);
}

MainWindow::~MainWindow() {
	SettingsHandler::getInstance().setValue("windowGeometry/width", this->geometry().width());
	SettingsHandler::getInstance().setValue("windowGeometry/height", this->geometry().height());
	SettingsHandler::getInstance().setValue("windowGeometry/positionX", this->geometry().x());
	SettingsHandler::getInstance().setValue("windowGeometry/positionY", this->geometry().y());
	
	SettingsHandler::getInstance().sync();
	delete ui;
}

void MainWindow::pauseGame() {
	StateHandler::getInstance().setGamePaused(!StateHandler::getInstance().isGamePaused());
}


void MainWindow::newGame() {
	this->ui->graphicsView->setSceneRect(0, 0, this->getSceneDimension(), this->getSceneDimension());
	PlayerInfo playerA = this->newGameDialog.getPlayerInfo(1);
	PlayerInfo playerB = this->newGameDialog.getPlayerInfo(0);
	
	this->lastPlayerA = playerA;
	this->lastPlayerB = playerB;
	
	if (!loading) {
		if (GameHandler::getInstance().newGame(playerA, playerB, this->getSceneDimension() / 7.0, 
			!StateHandler::getInstance().isEditorMode()) == true)
			this->playerChanged();
		else {
			QMessageBox msgBox;
			msgBox.critical(this, "What a shame...", QString("An error occured when trying to create the new game.") + 
				((playerA.botPath.size() > 0 || playerB.botPath.size() > 0) ? 
					" Protip: You might want to change the bot you're using." : ""));
			
			StateHandler::getInstance().gameFinished();
		}
	}
}

void MainWindow::loadGame() {
	this->loading = true;
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Load game"), SAVES_DIR, tr("Diaballik save files (*.sav)"));
	
	if (filename.length() == 0)
		return;
	
	this->newGameDialog.exec();
	
	QMessageBox msgBox;
	msgBox.setWindowTitle("Error!");
	msgBox.setText("There was an error loading the game! Your save might be corrupted.");
	
	if(!GameHandler::getInstance().loadGame(filename, this->lastPlayerA, this->lastPlayerB, 
		this->getSceneDimension() / 7.0))
		msgBox.exec();
	
	this->loading = false;
	this->playerChanged();
}

void MainWindow::saveGame() {
	QString filename = QFileDialog::getSaveFileName(this, 
		tr("Save game"), SAVES_DIR, tr("Diaballik save files (*.sav)"));
	
	if (filename.length() == 0)
		return;
	
 	bool res = GameHandler::getInstance().saveGame(filename);
	assert(res);
}


void MainWindow::playerChanged() {
	QString activity = ((GameHandler::getInstance().getPlayerType() == AI_PLAYER &&
	StateHandler::getInstance().isGamePaused() == false) ? " thinking..."  : "");	//I know, It's a hack ;p
	
	this->ui->statusBar->showMessage("Current player: " + GameHandler::getInstance().getPlayerName() + activity);
	this->moveFinished();
	StateHandler::getInstance().playerChanged();
}

void MainWindow::moveFinished() {
	QString txt = QString("Moves left: %1, Passes left: %2").arg(GameHandler::getInstance().getMovesLeft()).arg(
			      GameHandler::getInstance().getPassesLeft());
	this->ui->movesLeftLabel->setText(txt);
}

void MainWindow::gameFinished() {
	StateHandler::getInstance().gameFinished();
	QString name = GameHandler::getInstance().getWinnerName();
	this->ui->statusBar->showMessage("Game finished " + 
	((name.size() == 0) ? "It's a draw!" : "The " + name + " player won!"));
}


void MainWindow::startGameEditor() {
	GameHandler::getInstance().startEditor();
}

void MainWindow::showRules() {
	QMessageBox msgBox;
	msgBox.information(this, "[This] Game rules", RULES_MESSAGE);
}

void MainWindow::showAuthor() {
	QMessageBox msgBox;
	msgBox.about(this, "Author", AUTHOR_MESSAGE);
}

void MainWindow::showHelp() {
	QMessageBox msgBox;
	msgBox.information(this, "Game help", HELP_MESSAGE);
}

void MainWindow::showSettings() {
	this->displayError(QString("I didn't manage to implement the proper settings (I ran out of time). Everything you might want to edit thought is ") +
		"at $HOME$/.diaballik/config.ini . Happy hacking!");
}

void MainWindow::displayError (const QString msg) {
	QMessageBox msgBox;
	msgBox.critical(this, "Error!", msg);
}