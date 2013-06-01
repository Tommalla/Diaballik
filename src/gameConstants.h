#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Global contants that we cannot move to a settings file */

#include <QString>

const QString AUTHOR = "Tomasz [Tommalla] Zakrzewski, tz336079@students.mimuw.edu.pl";
const QString VERSION = "v0.1 beta";
const QString APP_NAME = "Diaballik " + VERSION;

const QString APP_LOCATION = "~/.diaballik/";
const QString SETTINGS_FILE = APP_LOCATION + "config.ini";

#endif