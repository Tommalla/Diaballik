#ifndef GAME_ENUMS_H
#define GAME_ENUMS_H

/**
 * @brief Defines possible player types
 **/
enum ApplicationPlayerType {
	HUMAN_PLAYER = 2,
	AI_PLAYER = 3
	//NETWORK_PLAYER	//future idea
};

enum ApplicationState {
	GAME_FINISHED = 0,
	BOARD_EDITOR = 1,
	HUMAN_MOVE = HUMAN_PLAYER,
	AI_MOVE = AI_PLAYER
};

#endif