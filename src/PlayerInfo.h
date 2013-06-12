#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <QString>
#include "gameEnums.h"

struct PlayerInfo {
	ApplicationPlayerType type;
	QString botPath;
	QString name;
};

static const PlayerInfo make_human_player(const QString& name) {
	PlayerInfo res;
	res.type = HUMAN_PLAYER;
	res.name = name;
	return res;
}
// 
// static const PlayerInfo make_ai_player(const QString& path) {
// 	PlayerInfo res;
// 	res.type = AI_PLAYER;
// 	res.botPath = path;
// 	return res;
// }

#endif