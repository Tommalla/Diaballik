/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "HumanPlayer.h"

HumanPlayer::HumanPlayer() {
	ready = true;
}

void HumanPlayer::endGame ( bool win )
{
	//TODO, możliwie nic nie jest do zrobienia
// 	Player::endGame ( win );
}

void HumanPlayer::setMove ( const Move& move ) {
	this->move = move;
}


