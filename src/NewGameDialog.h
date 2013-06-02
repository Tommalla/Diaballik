#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <QDialog>
#include "gameEnums.h"

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
		void setWidgetUsable(QWidget* widget, const bool state = true);
    
	public:
		explicit NewGameDialog(QWidget *parent = 0);
		const ApplicationPlayerType getWhitePlayerType() const;
		const ApplicationPlayerType getBlackPlayerType() const;
		~NewGameDialog();
		
	private slots:
		void changeWhiteGroupBox();
		void changeBlackGroupBox();
};

#endif // NEWGAMEDIALOG_H
