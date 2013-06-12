#include "NewGameDialog.h"
#include "ui_NewGameDialog.h"

//FIXME this is not the best implementation possible... (lots of delicious pasta)

void NewGameDialog::setWidgetUsable (QWidget* widget, const bool state) {
	widget->setEnabled(state);
	widget->setVisible(state);
}

const ApplicationPlayerType NewGameDialog::getBlackPlayerType() const {
	return (ApplicationPlayerType)this->ui->blackComboBox->itemData(this->ui->blackComboBox->currentIndex()).toInt();
}

const ApplicationPlayerType NewGameDialog::getWhitePlayerType() const {
	return (ApplicationPlayerType)this->ui->whiteComboBox->itemData(this->ui->whiteComboBox->currentIndex()).toInt();
}

NewGameDialog::NewGameDialog(QWidget *parent) : QDialog(parent), ui(new Ui::NewGameDialog) {
	this->ui->setupUi(this);
	
	//TODO make it shrink...
	//this->ui->groupBox->setSizePolicy(Qt::MinimumSize);
	//this->ui->groupBox_2->setSizePolicy(Qt::MinimumSize);
	
	this->setWidgetUsable(this->ui->blackListWidget, false);
	this->setWidgetUsable(this->ui->whiteListWidget, false);
	
	//create entries in comboBoxes
	QComboBox* tempComboBoxTab[2] = { this->ui->blackComboBox, this->ui->whiteComboBox };
	
	for (QComboBox* comboBox: tempComboBoxTab) {
		comboBox->addItem("Human", HUMAN_PLAYER);
		comboBox->addItem("AI Player", AI_PLAYER);
	}

	//fill bots list
	QListWidget* tempListWidgetTab[2] = { this->ui->whiteListWidget, this->ui->blackListWidget };
	
	for (QListWidget* listWidget: tempListWidgetTab) {
		listWidget->addItem("Here be bots! (in the future)");
	}
	
	QObject::connect(this->ui->blackComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeBlackGroupBox()));
	QObject::connect(this->ui->whiteComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeWhiteGroupBox()));
}

const PlayerInfo NewGameDialog::getPlayerInfo(const int id) const {
	switch ( ((id == 0) ? this->getWhitePlayerType() : this->getBlackPlayerType()) ) {
		case HUMAN_PLAYER:
			return make_human_player((id == 0) ? "white" : "black");
			break;
		case AI_PLAYER:
			//TODO: return make_ai_player()
			break;
	}
}

NewGameDialog::~NewGameDialog() {
	delete ui;
}

void NewGameDialog::changeWhiteGroupBox () {
	switch (this->getWhitePlayerType()) {
		case HUMAN_PLAYER:
			this->setWidgetUsable(this->ui->whiteListWidget, false);
			break;
		case AI_PLAYER:
			this->setWidgetUsable(this->ui->whiteListWidget, true);
			break;
	}
}

void NewGameDialog::changeBlackGroupBox () {
	switch (this->getBlackPlayerType()) {
		case HUMAN_PLAYER:
			this->setWidgetUsable(this->ui->blackListWidget, false);
			break;
		case AI_PLAYER:
			this->setWidgetUsable(this->ui->blackListWidget, true);
			break;
	}
}
