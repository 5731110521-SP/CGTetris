#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <set>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "block.h"

#define ROW 20
#define COLUMN 10
#define DEPTH 10

using namespace std;

class Board {
public:
	bool board[ROW][COLUMN][DEPTH]; //have something on board?
	bool boardCurrent[ROW][COLUMN][DEPTH];

	bool boardShadow[ROW][COLUMN][DEPTH];
	Block blocks[ROW][COLUMN][DEPTH]; //blocks on the board
	int currentPointRow;
	int currentPointColumn;
	int currentPointDepth;
	vector<int> shadowrow;
	vector<int> shadowcolumn;
	vector<int> shadowDepth;
	vector<int> currentRow;
	vector<int> currentColumn;
	vector<int> currentDepth;

	Board();

	void addblocks(Block b, int row, int column,int depth);
	bool canmoveblock(int row, int column, int depth);
	void moveblock(int column,int depth);

	bool canmoveShadow(int row, int column);
	bool movedownShadow();

	pair<int, int> getRotateTo(int row, int column);
	bool canRotateBlock();
	void rotateBlock();

	bool movedown();
	void update();
};

#endif
