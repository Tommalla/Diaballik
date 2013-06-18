#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <QString>
#include "gameEnums.h"

struct PlayerInfo {
	ApplicationPlayerType type;
	QString botPath;
	QString name;
};

const PlayerInfo make_human_player(const QString& name);
const PlayerInfo make_ai_player(const QString& path, const QString& name);

#endif