#pragma once
#include"Chess.h"

class Ai
{
public:

	//初始化
	void init(Chess* chess);

	//落子
	void go();

private:

	//存储棋盘
	Chess* chess;

	//价值统计
	vector<vector<int>>scoreMap;

	//计算价值
	void calculateScore();

	//思考
	ChessPos think();
};
