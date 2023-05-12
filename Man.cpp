#include "Man.h"

void Man::init(Chess* chess)
{
	this->chess = chess;
}

void Man::go()
{
	//存储正确的点击信息
	ChessPos pos;

	//一直获取，直到信息有效
	while (true)
	{
		//获取点击信息
		ExMessage msg = getmessage();

		//通过chess对象的clickBoard方法判断是否有效，若有效，则落子
		if (msg.message == WM_LBUTTONDOWN && chess->clickBoard(msg.x, msg.y, &pos))
		{
			break;
		}
	}

	//落子
	chess->chessDown(&pos, CHESS_BLACK);
}
