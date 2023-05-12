#include "Game.h"

Game::Game(Man* man, Ai* ai, Chess* chess)
{
	this->man = man;
	this->ai = ai;
	this->chess = chess;

	man->init(chess);
	ai->init(chess);
}

void Game::play()
{
	//棋盘初始化
	chess->init();

	while (true)
	{
		//先棋手走
		man->go();
		if (chess->checkOver())
		{
			closegraph();

			//是否继续
			chess->exitGame() ? exit(EXIT_FAILURE) : chess->init();

			continue;
		}

		//后ai走
		ai->go();
		if (chess->checkOver())
		{
			closegraph();

			//是否继续
			chess->exitGame() ? exit(EXIT_FAILURE) : chess->init();

			continue;
		}
	}
}
