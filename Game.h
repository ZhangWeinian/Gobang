#pragma once
#include "Man.h"
#include "Ai.h"
#include "Chess.h"

class Game
{
public:

	//初始化
	Game(Man* man, Ai* ai, Chess* chess);

	//开始对局
	void play();

private:

	//添加数据
	Man* man;
	Ai* ai;
	Chess* chess;
};
