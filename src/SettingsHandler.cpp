/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "gameConstants.h"
#include "SettingsHandler.h"

SettingsHandler::SettingsHandler() : QSettings(SETTINGS_FILE, QSettings::IniFormat) {
	//this method is a STUB, we only need to call the parent constructor
}

