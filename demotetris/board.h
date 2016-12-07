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
#define DEPTH 5

using namespace std;

class Board {
public:
	bool board[ROW][COLUMN][DEPTH]; //have something on board?
	bool boardCurrent[ROW][COLUMN][DEPTH];

<<<<<<< HEAD
	bool boardShadow[ROW][COLUMN][DEPTH];
	Block blocks[ROW][COLUMN][DEPTH]; //blocks on the board
=======
	bool isLastHit = false,isHit;
	int digit1=0,digit2=0,combo=1,score =0;
	bool boardShadow[20][10];
	Block blocks[20][10]; //blocks on the board
>>>>>>> refs/remotes/origin/DA
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

<<<<<<< HEAD
	void addblocks(Block b, int row, int column,int depth);
	bool canmoveblock(int row, int column, int depth);
	void moveblock(int column,int depth);
=======

	void getScore();

	void addblocks(Block b, int row, int column);
	bool onCurrent(int row, int column);
	bool canmoveblock(int row, int column);
	void moveblock(int left);
>>>>>>> refs/remotes/origin/DA

	bool canmoveShadow(int row, int column);
	bool movedownShadow();

	pair<int, int> getRotateTo(int row, int column);
	bool canRotateBlock();
	void rotateBlock();

	bool movedown();
	void removeCurrent();
	void update();
};

#endif
