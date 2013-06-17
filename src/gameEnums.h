#ifndef GAME_ENUMS_H
#define GAME_ENUMS_H

/**
 * @brief Defines possible player types
 **/
enum ApplicationPlayerType {
	HUMAN_PLAYER = 2,
	AI_PLAYER = 3,
	NO_PLAYER = 0
	//NETWORK_PLAYER	//future idea
};

enum ApplicationState {
	GAME_FINISHED = NO_PLAYER,
	BOARD_EDITOR = 1,
	HUMAN_MOVE = HUMAN_PLAYER,
	AI_MOVE = AI_PLAYER
};

#endif