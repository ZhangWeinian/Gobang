#pragma once
#include<easyx.h>//easyx图形库
#include<vector>

using namespace std;

//位置坐标
struct ChessPos
{
	int row;
	int col;

	ChessPos(int row = 0, int col = 0) :row(row), col(col)
	{
	}
};

//落子类型
typedef enum
{
	//黑子
	CHESS_BLACK = 1,

	//白子
	CHESS_WHITE = -1
}chess_kind_t;

class Chess
{
public:
	Chess(int gradeSize, int marginX, int marginY, double chessSize);

	//初始化：加载图片资源，初始化棋盘数据
	void init();

	//获取大小和指定位置的信息
	int getGradeSize();
	int getGradeSize(int roe, int col);
	int getGradeSize(ChessPos* pos);

	//判断位置是否有效
	bool clickBoard(int x, int y, ChessPos* pos);

	//棋子去边框，x为载入图片的X坐标，y为Y坐标
	void putimagePNG(int x, int y, IMAGE* picture);

	//落子
	void chessDown(ChessPos* pos, chess_kind_t kind);

	//是否结束
	bool checkOver();

	//退出返回true，否则返回false
	bool exitGame();

private:

	//刷新棋盘
	void updateGameMap(ChessPos* pos);

	//若胜负已分，返回true；否则返回false
	bool checkWin();

	//黑子
	IMAGE chessBlackImg;

	//白子
	IMAGE chessWhiteImg;

	//棋盘大小（13，15，17，19）
	int gradeSize;

	//棋盘左侧边缘
	int margin_x;

	//棋盘上侧边缘
	int margin_y;

	//棋子大小
	double chessSize;

	//最后落子位置
	ChessPos lastPos;

	//当前棋局的棋子分布数据
	//0：空白， 1：黑子， -1：白子
	vector<vector<int>>chessMap;

	//表示现在该谁下子
	//true：黑子， false：白子
	bool playerFlag;
};
