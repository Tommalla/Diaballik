#include "PlayerInfo.h"

const PlayerInfo make_human_player(const QString& name) {
	PlayerInfo res;
	res.type = HUMAN_PLAYER;
	res.name = name;
	return res;
}

const PlayerInfo make_ai_player(const QString& path, const QString& name) {
	PlayerInfo res;
	res.type = AI_PLAYER;
	res.botPath = path;
	res.name = name;
 	return res;
}