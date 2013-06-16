#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

/* Global contants that we cannot move to a settings file */

#include <QString>
#include <QDir>
#include <QColor>
#include <QPen>

const QString AUTHOR = "Tomasz [Tommalla] Zakrzewski";
const QString AUTHOR_MAIL = "tz336079@students.mimuw.edu.pl";
const QString VERSION = "v0.1 alpha";
const QString APP_NAME = "Diaballik " + VERSION;

const QString AUTHOR_MESSAGE = "<center>" + APP_NAME + "<br/>" +AUTHOR + "<br/><a href=\"mailto:" + AUTHOR_MAIL + "\">" + AUTHOR_MAIL + 
"</a><br/>IPP 2012/2013 <a href=\"http://www.mimuw.edu.pl\">MIMUW</a></center>";

const QString HELP_MESSAGE = "Keyboard shortcuts to appear in the future";

const QString RULES_MESSAGE = QString("<h3>Rules</h3>") +
	"<p>Diaballik's rules are simple. Each opponent has a team composed of seven players, one of which has a ball. There is no " + 
	"mechanism to remove players, so there are always 14 players in the game.</p>" +
	"<p>The goal of the game is to bring a player to the opponent's side with the ball.</p>"+
	"<p>Opponents take turns and can optionally take three actions during their turn in any " +
	"order; two moves and one ball throw. A move is to move one player just one horizontal or " +
	"vertical space.</p> <p>The player with the ball can throw his ball to another player of his " +
	"team who is located in the vertical line, the horizontal line or a diagonal line and if no " +
	"opposing player is in the way. As in handball, the player with the ball cannot move while he " +
	"has the ball. So to move, the ball holder must pass to one of his partners. Moves can only be " +
	"made in the horizontal or vertical direction and only if the destination is unoccupied. Moving " +
	"diagonally requires two moves, since one must move horizontally, then vertically, in two stages " +
	"(as such one cannot move diagonally between two pieces).</p><h3>Anti-game rule</h3><p>Opponents " +
	"need to bring at least one of their players to the other side. If all players of a team are adjacent," + 
	"one per column, the adverse team cannot reach the adverse side, because they form an uncrossable line." +
	" The \"anti-game\" rule dissuades opponents to make an uncrossable line. In practice the \"anti-game\" " +
	"rule specifies that if an opponent puts its players in a uncrossable line and at least three opposing " +
	"players are adjacent to this line, then the opponent players wins.</p>" +
	"<p>Source: <a href=\"http://en.wikipedia.org/wiki/Diaballik\">wikipedia</a>";

const QString APP_LOCATION = QDir::homePath() + "/.diaballik/";
const QString SETTINGS_FILE = APP_LOCATION + "config.ini";
const QString SAVES_DIR = APP_LOCATION + "saves/";

const QColor DEFAULT_PRIMARY_SELECTION_COLOR = QColor::fromRgb(12, 232, 12);
const QColor DEFAULT_SECONDARY_SELECTION_COLOR = QColor::fromRgb(205, 201, 201);
const QPen DEFAULT_PRIMARY_SELECTION_PEN = QPen(Qt::DashLine);

const int DEFAULT_PLAYERS_TIMER_INTERVAL = 100;
const int DEFAULT_ANIMATION_DURATION = 100;

const int PLAYERS_QTY = 2;

const int SANE_HISTORY_SIZE = 100000;

#endif
