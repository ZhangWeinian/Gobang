#include "Chess.h"
#include<math.h>
#include<mmsystem.h>//提供音乐播放
#pragma comment(lib,"winmm.lib")//提供音乐播放

Chess::Chess(int gradeSize, int marginX, int marginY, double chessSize)
{
	//初始化图片数据
	this->gradeSize = gradeSize;
	this->margin_x = marginX;
	this->margin_y = marginY;
	this->chessSize = chessSize;

	//黑方先手
	playerFlag = static_cast<bool>(CHESS_BLACK);

	//初始化棋盘（全为0）
	for (size_t i = 0; i < gradeSize; i++)
	{
		vector<int>cmp;
		for (size_t j = 0; j < gradeSize; j++)
		{
			cmp.emplace_back(0);
		}

		chessMap.emplace_back(cmp);
	}
}

void Chess::init()
{
	//创建窗口
	initgraph(825, 825);

	//显示背景图片
	loadimage(nullptr, L"res/chess.png");

	//加载棋子图片到内存
	loadimage(&chessBlackImg, L"res/black.png", static_cast<int>(chessSize) + 1, static_cast<int>(chessSize) + 1, true);
	loadimage(&chessWhiteImg, L"res/white.png", static_cast<int>(chessSize) + 1, static_cast<int>(chessSize) + 1, true);

	//播放背景音乐
	mciSendString(L"play res/go.mp3", 0, 0, 0);

	//棋盘清零
	size_t num = 0;
	for (auto& m : chessMap.at(num++))
	{
		for (auto& n : chessMap.at(num))
		{
			n = 0;
		}
	}

	//黑子先手
	playerFlag = true;
}

int Chess::getGradeSize()
{
	return gradeSize;
}

int Chess::getGradeSize(int row, int col)
{
	return chessMap.at(row).at(col);
}

int Chess::getGradeSize(ChessPos* pos)
{
	return chessMap.at(pos->row).at(pos->col);
}

bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	bool ans = false;

	//距离最近的正确坐标
	int col = static_cast<int>((x - margin_x) / chessSize);
	int row = static_cast<int>((y - margin_y) / chessSize);

	//上述正确坐标的像素位置
	int leftTopPosX = static_cast<int>(margin_x + chessSize * col);
	int leftTopPosY = static_cast<int>(margin_y + chessSize * row);

	//允许误差
	int offset = static_cast<int>(chessSize * 0.2);

	//横纵误差
	int dx = x - leftTopPosX;
	int dy = y - leftTopPosY;

	//正确坐标与真实坐标间距
	int len = 0;

	do
	{
		//左上角判断
		len = static_cast<int>(sqrt(dx * dx - dy * dy));
		if (len < offset)
		{
			pos->row = row;
			pos->col = col;

			if (chessMap.at(pos->row).at(pos->col) == 0)
			{
				ans = true;
			}

			break;
		}

		//右上角判断(
		len = static_cast<int>(sqrt((dx - chessSize) * (dx - chessSize) - dy * dy));
		if (len < offset)
		{
			pos->row = row;
			pos->col = col + 1;

			if (chessMap.at(pos->row).at(pos->col) == 0)
			{
				ans = true;
			}

			break;
		}

		//左下角判断
		len = static_cast<int>(sqrt(dx * dx - (dy - chessSize) * (dy - chessSize)));
		if (len < offset)
		{
			pos->row = row + 1;
			pos->col = col;

			if (chessMap.at(pos->row).at(pos->col) == 0)
			{
				ans = true;
			}

			break;
		}

		//右下角判断
		len = static_cast<int>(sqrt((dx - chessSize) * (dx - chessSize) - (dy - chessSize) * (dy - chessSize)));
		if (len < offset)
		{
			pos->row = row + 1;
			pos->col = col + 1;

			if (chessMap.at(pos->row).at(pos->col) == 0)
			{
				ans = true;
			}

			break;
		}
	} while (false);

	return ans;
}

void Chess::putimagePNG(int x, int y, IMAGE* picture)
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
	int x = static_cast<int>(margin_x + chessSize * pos->col - 0.5 * chessSize);
	int y = static_cast<int>(margin_y + chessSize * pos->row - 0.5 * chessSize);

	mciSendString(L"play res/down.mp3", 0, 0, 0);

	if (kind == CHESS_WHITE)
	{
		putimagePNG(x, y, &chessWhiteImg);
	}
	else
	{
		putimagePNG(x, y, &chessBlackImg);
	}

	updateGameMap(pos);
}

void Chess::updateGameMap(ChessPos* pos)
{
	//更新最落子位置
	lastPos = *pos;

	//将棋子信息录入chessMap
	chessMap.at(pos->row).at(pos->col) = playerFlag ? CHESS_BLACK : CHESS_WHITE;

	//交换
	playerFlag = !playerFlag;
}

bool Chess::checkWin()
{
	int row = lastPos.row;
	int col = lastPos.col;

	//水平方向
	for (int i = 0; i < 5; i++)
	{
		if (col - i >= 0 && col - i + 4 < gradeSize &&
			chessMap.at(row).at(col - i) == chessMap.at(row).at(col - i + 1) &&
			chessMap.at(row).at(col - i) == chessMap.at(row).at(col - i + 2) &&
			chessMap.at(row).at(col - i) == chessMap.at(row).at(col - i + 3) &&
			chessMap.at(row).at(col - i) == chessMap.at(row).at(col - i + 4))
		{
			return true;
		}
	}

	//垂直方向
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 && row - i + 4 < gradeSize &&
			chessMap.at(row - i).at(col) == chessMap.at(row - i + 1).at(col) &&
			chessMap.at(row - i).at(col) == chessMap.at(row - i + 2).at(col) &&
			chessMap.at(row - i).at(col) == chessMap.at(row - i + 3).at(col) &&
			chessMap.at(row - i).at(col) == chessMap.at(row - i + 4).at(col))
		{
			return true;
		}
	}

	//‘/’方向
	for (int i = 0; i < 5; i++)
	{
		if (row + i < gradeSize && row + i - 4 >= 0 &&
			col - i >= 0 && col - i + 4 < gradeSize &&
			chessMap.at(row + i).at(col - i) == chessMap.at(row + i - 1).at(col - i + 1) &&
			chessMap.at(row + i).at(col - i) == chessMap.at(row + i - 2).at(col - i + 2) &&
			chessMap.at(row + i).at(col - i) == chessMap.at(row + i - 3).at(col - i + 3) &&
			chessMap.at(row + i).at(col - i) == chessMap.at(row + i - 4).at(col - i + 4))
		{
			return true;
		}
	}

	//‘\’方向
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 && row - i + 4 < gradeSize &&
			col - i >= 0 && col - i + 4 < gradeSize &&
			chessMap.at(row - i).at(col - i) == chessMap.at(row - i + 1).at(col - i + 1) &&
			chessMap.at(row - i).at(col - i) == chessMap.at(row - i + 2).at(col - i + 2) &&
			chessMap.at(row - i).at(col - i) == chessMap.at(row - i + 3).at(col - i + 3) &&
			chessMap.at(row - i).at(col - i) == chessMap.at(row - i + 4).at(col - i + 4))
		{
			return true;
		}
	}

	return false;
}

bool Chess::checkOver()
{
	if (checkWin())
	{
		Sleep(500);

		//此时该黑方（棋手）落子，则白子（ai）落下后取胜，棋手失败
		if (playerFlag == true)
		{
			mciSendString(L"play res/失败.mp3", 0, 0, 0);
			loadimage(0, L"res/失败.jpg");
		}

		//反之则棋手胜利
		else
		{
			mciSendString(L"play res/胜利.mp3", 0, 0, 0);
			loadimage(0, L"res/胜利.jpg");
		}

		Sleep(1500);

		return true;
	}

	return false;
}

bool Chess::exitGame()
{
	//MOUSEMSG msg = GetMouseMsg();

	return true;
}
