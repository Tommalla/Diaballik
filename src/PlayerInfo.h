#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <QString>
#include "gameEnums.h"
#include "../DiaballikEngine/src/enums.h"

struct PlayerInfo {
	ApplicationPlayerType type;
	QString botPath;
	bool GTPE;
	QString name;
	GamePlayer player;
};

const PlayerInfo make_human_player(const QString& name);
const PlayerInfo make_ai_player(const QString& path, const QString& name, const bool GTPE = false);

#endif