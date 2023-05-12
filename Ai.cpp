#include "Ai.h"
#include <ctime>

void Ai::init(Chess* chess)
{
	this->chess = chess;

	int size = chess->getGradeSize();
	for (size_t i = 0; i < size; i++)
	{
		vector<int>cmp;

		for (size_t j = 0; j < size; j++)
		{
			cmp.emplace_back(0);
		}

		scoreMap.emplace_back(cmp);
	}
}

void Ai::go()
{
	//选出最大价值
	ChessPos pos = think();

	//假装思考
	Sleep(700);

	//落子
	chess->chessDown(&pos, CHESS_WHITE);
}

void Ai::calculateScore()
{
	//评分向量数组清零
	size_t num = 0;
	for (auto& m : scoreMap.at(num++))
	{
		for (auto& n : scoreMap.at(num))
		{
			n = 0;
		}
	}

	//开始评分

	//记录长度
	int size = chess->getGradeSize();

	//棋手（黑）有多少个连续的子
	int persinNum = 0;

	//ai（白）有多少个连续的子
	int aiNum = 0;

	//该方向的空白位个数
	int emptyNum = 0;

	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
			//对每个点进行计算
			if (chess->getGradeSize(row, col) != 0)
			{
				continue;
			}

			for (int y = -1; y < 1; y++)
			{
				for (int x = -1; x < 2; x++)
				{
					//跳过本身位置
					if (x == 0 && y == 0)
					{
						continue;
					}

					//取消重复判断
					if (y == 0 && x != 1)
					{
						continue;
					}

					//重置
					persinNum = 0;
					aiNum = 0;
					emptyNum = 0;

					//假设黑子在此处落子，会构成何种棋形
					for (int i = 1; i < 5; i++)
					{
						int curRow = row + i * y;
						int curCol = col + i * x;

						if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getGradeSize(curRow, curCol) == CHESS_BLACK)
						{
							persinNum++;
						}
						else if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getGradeSize(curRow, curCol) == 0)
						{
							emptyNum++;
							break;
						}
						else
						{
							break;
						}
					}

					//反向继续判断
					for (int i = 1; i < 5; i++)
					{
						int curRow = row - i * y;
						int curCol = col - i * x;

						if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getGradeSize(curRow, curCol) == 1)
						{
							persinNum++;
						}
						else if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getGradeSize(curRow, curCol) == 0)
						{
							emptyNum++;
							break;
						}
						else
						{
							break;
						}
					}

					//录入分数
					//连2
					if (persinNum == 1)
					{
						scoreMap.at(row).at(col) += 10;
					}

					//连3
					if (persinNum == 2)
					{
						//死3
						if (emptyNum == 1)
						{
							scoreMap.at(row).at(col) += 30;
						}

						//活3
						if (emptyNum == 2)
						{
							scoreMap.at(row).at(col) += 40;
						}
					}

					//连4
					if (persinNum == 3)
					{
						//死4
						if (emptyNum == 1)
						{
							scoreMap.at(row).at(col) += 60;
						}

						//活4
						if (emptyNum == 2)
						{
							scoreMap.at(row).at(col) += 200;
						}
					}

					//连5
					if (persinNum == 4)
					{
						scoreMap.at(row).at(col) += 20000;
					}

					//假设白子在此处落子，会构成何种棋形
					emptyNum = 0;
					for (int i = 1; i < 5; i++)
					{
						int curRow = row + i * y;
						int curCol = col + i * x;

						if (curRow > 0 && curRow < size && curCol > 0 && curCol < size && chess->getGradeSize(curRow, curCol) == CHESS_WHITE)
						{
							aiNum++;
						}
						else if (curRow > 0 && curRow < size && curCol > 0 && curCol < size && chess->getGradeSize(curRow, curCol) == 0)
						{
							emptyNum++;
							break;
						}
						else
						{
							break;
						}
					}

					//反向继续判断
					for (int i = 1; i < 5; i++)
					{
						int curRow = row - i * y;
						int curCol = col - i * x;

						if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getGradeSize(curRow, curCol) == 1)
						{
							persinNum++;
						}
						else if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getGradeSize(curRow, curCol) == 0)
						{
							emptyNum++;
							break;
						}
						else
						{
							break;
						}
					}

					//录入分数
					if (aiNum == 0)
					{
						scoreMap.at(row).at(col) += 5;
					}

					//连2
					if (aiNum == 1)
					{
						scoreMap.at(row).at(col) += 10;
					}

					//连3
					if (aiNum == 2)
					{
						//死3
						if (emptyNum == 1)
						{
							scoreMap.at(row).at(col) += 25;
						}

						//活3
						if (emptyNum == 2)
						{
							scoreMap.at(row).at(col) += 50;
						}
					}

					//连4
					if (aiNum == 3)
					{
						//死4
						if (emptyNum == 1)
						{
							scoreMap.at(row).at(col) += 55;
						}

						//活4
						if (emptyNum == 2)
						{
							scoreMap.at(row).at(col) += 10000;
						}
					}

					//连5
					if (aiNum >= 4)
					{
						scoreMap.at(row).at(col) += 30000;
					}

					if (aiNum == 5)
					{
					}
				}
			}
		}
	}
}

ChessPos Ai::think()
{
	//先计算每个点的价值
	calculateScore();

	int size = chess->getGradeSize();

	//最大值
	int maxScore = 0;

	//存储相同最值的所有点
	vector<ChessPos>maxPoints(0);

	//寻找最大值并加入maxPoints
	for (size_t row = 0; row < size; row++)
	{
		for (size_t col = 0; col < size; col++)
		{
			//前提是这个坐标是空的
			if (chess->getGradeSize(row, col) == 0)
			{
				if (maxScore < scoreMap.at(row).at(col))
				{
					maxScore = scoreMap.at(row).at(col);
					maxPoints.clear();
					maxPoints.emplace_back(ChessPos(row, col));
				}
				if (maxScore == scoreMap.at(row).at(col))
				{
					maxPoints.emplace_back(ChessPos(row, col));
				}
			}
		}
	}

	//在maxPoints中随机选取一点落子
	srand((int) time(nullptr));
	int index = rand() % maxPoints.size();

	return maxPoints.at(index);
}
