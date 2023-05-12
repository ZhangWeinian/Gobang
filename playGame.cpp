#include<iostream>
#include"Game.h"

int main(void)
{
	Chess chess(15, 37, 37, 53.643);
	Man man {};
	Ai ai;

	Game game(&man, &ai, &chess);

	game.play();

	return 0;
}
