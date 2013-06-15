#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

/* Global contants that we cannot move to a settings file */

#include <QString>
#include <QDir>
#include <QColor>
#include <QPen>

const QString AUTHOR = "Tomasz [Tommalla] Zakrzewski, tz336079@students.mimuw.edu.pl";
const QString VERSION = "v0.1 alpha";
const QString APP_NAME = "Diaballik " + VERSION;

const QString APP_LOCATION = QDir::homePath() + "/.diaballik/";
const QString SETTINGS_FILE = APP_LOCATION + "config.ini";

const QColor DEFAULT_PRIMARY_SELECTION_COLOR = QColor::fromRgb(12, 232, 12);
const QColor DEFAULT_SECONDARY_SELECTION_COLOR = QColor::fromRgb(205, 201, 201);
const QPen DEFAULT_PRIMARY_SELECTION_PEN = QPen(Qt::DashLine);

const int DEFAULT_PLAYERS_TIMER_INTERVAL = 100;
const int DEFAULT_ANIMATION_DURATION = 100;

const int PLAYERS_QTY = 2;

const int SANE_HISTORY_SIZE = 100000;

#endif
