/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QSettings>
#include "../DiaballikEngine/src/Singleton.h"


/**
 * @brief A Singleton<> made of QSettings
 * All hail OOP!
 **/
class SettingsHandler : public QSettings, public Singleton<SettingsHandler> {
	friend class Singleton<SettingsHandler>;
	private:
		SettingsHandler();
};

#endif // SETTINGSHANDLER_H
