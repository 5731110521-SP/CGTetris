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

using namespace std;

class Board { 
  public:
	bool board[20][10]; //have something on board?
	bool boardCurrent[20][10];
	Block blocks[20][10]; //blocks on the board
	vector<int> currentRow;
	vector<int> currentColumn;

	Board();

	void addblocks(Block b,int row, int column);
	bool onCurrent(int row, int column);
	bool canmoveblock(int row,int column);
	void moveblock(int left);
	bool movedown();
	void update();
};

#endif
