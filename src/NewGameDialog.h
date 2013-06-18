#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <QDialog>
#include <vector>
#include "gameEnums.h"
#include "PlayerInfo.h"

using namespace std;

namespace Ui {
	class NewGameDialog;
}

/**
 * @brief A graphical New Game Dialog. Allows to choose players and set some
 * configuration for them.
 **/
class NewGameDialog : public QDialog {
	Q_OBJECT;
	
	private:
		Ui::NewGameDialog *ui;
		vector<PlayerInfo> bots; 
		
		void setWidgetUsable(QWidget* widget, const bool state = true);
		const ApplicationPlayerType getWhitePlayerType() const;
		const ApplicationPlayerType getBlackPlayerType() const;
    
	public:
		explicit NewGameDialog(QWidget *parent = 0);
		const PlayerInfo getPlayerInfo(const int id) const;
		~NewGameDialog();
		
	private slots:
		void changeWhiteGroupBox();
		void changeBlackGroupBox();
};

#endif // NEWGAMEDIALOG_H
