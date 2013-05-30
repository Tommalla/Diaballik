#include <QApplication>
#include <QTextCodec>
#include "MainWindow.h"

int main(int argc, char** argv) {	
	QApplication application(argc, argv);
	
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	
	MainWindow mainWindow;
	
	mainWindow.show();
	return application.exec();
}
