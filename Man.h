#pragma once
#include"Chess.h"

class Man
{
public:

	//初始化
	void init(Chess* chess);

	//落子
	void go();

private:
	Chess* chess;
};
