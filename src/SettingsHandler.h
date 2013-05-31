/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QSettings>
#include "../DiaballikEngine/src/Singleton.h"


class SettingsHandler : public QSettings, public Singleton<SettingsHandler> {
	private:
		SettingsHandler();
};

#endif // SETTINGSHANDLER_H
