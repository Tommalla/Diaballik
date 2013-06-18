#include <QDir>
#include <QSettings>
#include "NewGameDialog.h"
#include "ui_NewGameDialog.h"
#include "gameConstants.h"


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
	
	this->setWidgetUsable(this->ui->blackTreeWidget, false);
	this->setWidgetUsable(this->ui->whiteTreeWidget, false);
	
	//create entries in comboBoxes
	QComboBox* tempComboBoxTab[2] = { this->ui->blackComboBox, this->ui->whiteComboBox };
	
	for (QComboBox* comboBox: tempComboBoxTab) {
		comboBox->addItem("Human", HUMAN_PLAYER);
		comboBox->addItem("AI Player", AI_PLAYER);
	}

	//fill bots list
	QTreeWidget* tempListWidgetTab[2] = { this->ui->whiteTreeWidget, this->ui->blackTreeWidget };
	
	
	QObject::connect(this->ui->blackComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeBlackGroupBox()));
	QObject::connect(this->ui->whiteComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeWhiteGroupBox()));
	
	//TODO load the bots info
	QDir dir(BOTS_DIR);
	dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	
	QStringList list = dir.entryList();
	qDebug("List: %d", list.size());
	for (int i = 0; i < list.size(); ++i) {
		QString configPath = BOTS_DIR + "/" + list[i] + "/config.ini";
		QFile settings(configPath);
		
		if (settings.exists()) {
			qDebug("opened file!");
			QSettings botSettings(configPath, QSettings::IniFormat);
			QString exec = botSettings.value("Info/exec").toString();
			QString name = botSettings.value("Info/name").toString();
			bool GTPE = botSettings.value("Info/GTP-E", false).toBool();
			
			this->bots.push_back(make_ai_player(exec, name, GTPE));
			QStringList tmp;
			tmp.append(name);
			tmp.append((GTPE) ? "YES" : "NO");
			
			for (QTreeWidget* treeWidget: tempListWidgetTab) {
				treeWidget->addTopLevelItem(new QTreeWidgetItem(tmp));
			}
		}
	}
	
	for (QTreeWidget* treeWidget: tempListWidgetTab) {
		treeWidget->setRootIsDecorated(false);
		treeWidget->setCurrentItem(treeWidget->itemAt(0, 0));
	}
	
}

const PlayerInfo NewGameDialog::getPlayerInfo(const int id) const {
	switch ( ((id == 0) ? this->getWhitePlayerType() : this->getBlackPlayerType()) ) {
		case HUMAN_PLAYER:
			return make_human_player((id == 0) ? "white" : "black");
			break;
		case AI_PLAYER:
		{
			QTreeWidget* widget = (id == 0) ? this->ui->whiteTreeWidget : this->ui->blackTreeWidget;
			PlayerInfo res = this->bots[widget->currentIndex().row()];
			res.name = QString((id == 0) ? "white" : "black") + "(" + res.name + ")";
			return res;
		}
			break;
	}
}

NewGameDialog::~NewGameDialog() {
	delete ui;
}

void NewGameDialog::changeWhiteGroupBox () {
	switch (this->getWhitePlayerType()) {
		case HUMAN_PLAYER:
			this->setWidgetUsable(this->ui->whiteTreeWidget, false);
			break;
		case AI_PLAYER:
			this->setWidgetUsable(this->ui->whiteTreeWidget, true);
			break;
	}
}

void NewGameDialog::changeBlackGroupBox () {
	switch (this->getBlackPlayerType()) {
		case HUMAN_PLAYER:
			this->setWidgetUsable(this->ui->blackTreeWidget, false);
			break;
		case AI_PLAYER:
			this->setWidgetUsable(this->ui->blackTreeWidget, true);
			break;
	}
}
