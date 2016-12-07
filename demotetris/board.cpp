#include <vector>
#include "board.h"
#include "block.h"
#include "vec3f.h"


int randomColumn = 0;
int randomDepth = 0;

using namespace std;

Board::Board() {
	for (int i = 0; i < ROW; i++) for (int j = 0; j < COLUMN; j++) for (int k = 0; k < DEPTH; k++) {
		board[i][j][k] = false;
		boardCurrent[i][j][k] = false;
		boardShadow[i][j][k] = false;
	}
	/*Block block = Block();
	addblocks(block, 4, 0);
	addblocks(block, 5, 0);
	addblocks(block, 5, 1);
	addblocks(block, 6, 1);*/
}

void Board::addblocks(Block b, int row, int column,int depth)
{
	if (currentColumn.empty()) {
		srand(time(0));
		if (!COLUMN <= 4) randomColumn = rand() % (COLUMN - 4);
		else randomColumn = 0;
		randomDepth = rand() % DEPTH;

		currentPointRow = 0;
		currentPointColumn = randomColumn;
		currentPointDepth = randomDepth;
	}
	boardCurrent[row][column+randomColumn][depth + randomDepth] = true;
	blocks[row][column+randomColumn][depth + randomDepth] = b;
	currentRow.push_back(row);
	currentColumn.push_back(column + randomColumn);
	currentDepth.push_back(depth + randomDepth);

	boardShadow[row][column+randomColumn][depth + randomDepth] =true;
	shadowrow.push_back(row);
	shadowcolumn.push_back(column + randomColumn);
	shadowDepth.push_back(depth + randomDepth);

	if (shadowcolumn.size() == 4) {
		while (movedownShadow());
	}

	//add shadow
	//	int nrow=row;
	//    while(nrow<19&&!board[nrow+1][column]) nrow++;
	//    boardShadow[row][column]=false;
	//    boardShadow[nrow][column]=true;
	//    blocks[nrow][column]=b;
	//	shadowrow.pop_back();
	//    shadowrow.push_back(nrow);
	/*
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) cout << boardCurrent[i][j] << " ";
		cout << endl;
	}
	cout << endl;
	
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			for (int k = 0; k < DEPTH; k++) cout << board[i][j][k] << " ";
		}
		cout << endl;
	}
	cout << endl;
	*/
}

bool Board::canmoveblock(int row, int column, int depth)
{
	for (int i = 0; i < 4; i++) {
		//if (board[currentRow[i] + row][currentColumn[i] + column] && !onCurrent(currentRow[i] + row, currentColumn[i] + column)) return false;
		if (currentRow[i] + row<0 || currentRow[i] + row >= ROW
			|| currentColumn[i] + column<0 || currentColumn[i] + column >= COLUMN
			|| currentDepth[i] + depth<0 || currentDepth[i] + depth >= DEPTH
			|| board[currentRow[i] + row][currentColumn[i] + column][currentDepth[i] + depth]) return false;
	}
	return true;
}

void Board::moveblock(int column,int depth) //left=-1 right=1
{
	if (!canmoveblock(0, column, depth)) return;
	/*for (int i = 0; i < 4; i++) {
	//board[currentRow[i]][currentColumn[i]] = false;
	//board[currentRow[i]][currentColumn[i]+column] = true;
	boardCurrent[currentRow[i]][currentColumn[i]] = false;
	boardCurrent[currentRow[i]][currentColumn[i]+column] = true;
	blocks[currentRow[i]][currentColumn[i]+column] = blocks[currentRow[i]][currentColumn[i]];
	currentColumn[i] += column;
	}*/
	if (column == -1) {
		int k = 0;
		for (int j = 0; j < COLUMN; j++) {
			for (int i = 0; i < ROW; i++) {
				for (int l = currentDepth[0]; l < currentDepth[0]+1; l++) {
					if (boardCurrent[i][j][l]) {
						boardCurrent[i][j][l] = false;
						boardCurrent[i][j - 1][l] = true;
						blocks[i][j - 1][l] = blocks[i][j][l];
						k++;
						if (k >= 4) {
							currentPointColumn -= 1;
							for (int i = 0; i < 4; i++) currentColumn[i] -= 1;
							break;
						}
					}
				}
			}
		}
	}
	else if(column==1) {
		int k = 0;
		for (int j = COLUMN-1; j >= 0; j--) {
			for (int i = 0; i < ROW; i++) {
				for (int l = currentDepth[0]; l < currentDepth[0] + 1; l++) {
					if (boardCurrent[i][j][l]) {
						boardCurrent[i][j][l] = false;
						boardCurrent[i][j + 1][l] = true;
						blocks[i][j + 1][l] = blocks[i][j][l];
						k++;
						if (k >= 4) {
							currentPointColumn += 1;
							for (int i = 0; i < 4; i++) currentColumn[i] += 1;
							break;
						}
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			boardCurrent[currentRow[i]][currentColumn[i]][currentDepth[i]] = false;
			boardCurrent[currentRow[i]][currentColumn[i]][currentDepth[i]+depth] = true;
			blocks[currentRow[i]][currentColumn[i]][currentDepth[i] + depth]
				= blocks[currentRow[i]][currentColumn[i]][currentDepth[i]];
			currentDepth[i] += depth;
		}
	}

	for (int i = 0; i < 4; i++) {
		boardShadow[shadowrow[i]][shadowcolumn[i]][shadowDepth[i]] = false;
	}
	shadowrow.clear();
	shadowcolumn.clear();
	shadowDepth.clear();
	for (int i = 0; i < 4; i++) {
		shadowrow.push_back(currentRow[i]);
		shadowcolumn.push_back(currentColumn[i]);
		shadowDepth.push_back(currentDepth[i]);
		boardShadow[shadowrow[i]][shadowcolumn[i]][shadowDepth[i]] = true;
	}
	while (movedownShadow());
}

bool Board::canmoveShadow(int row, int column)
{
	for (int i = 0; i < 4; i++) {
		if (shadowrow[i] + row<0 || shadowrow[i] + row >= 20
			|| shadowcolumn[i] + column<0 || shadowcolumn[i] + column >= 10
			|| board[shadowrow[i] + row][shadowcolumn[i] + column][shadowDepth[i]]) return false;
	}
	return true;
}

bool Board::movedownShadow()
{
	if (!canmoveShadow(1, 0)) {
		return false;
	}
	/*
	int k = 0;
	for (int i = 19; i >= 0; i--) {
		for (int j = 0; j < 10; j++) {
			if (boardShadow[i][j][0]) {
				boardShadow[i][j][0] = false;
				boardShadow[i + 1][j][0] = true;
				k++;
				if (k >= 4) {
					for (int i = 0; i < 4; i++) shadowrow[i] += 1;
					return true;
				}
			}
		}
	}
	*/
	for (int i = 0; i < 4; i++) {
		boardShadow[shadowrow[i]][shadowcolumn[i]][shadowDepth[i]] = false;
		shadowrow[i] += 1;
	}
	for (int i = 0; i < 4; i++) {
		boardShadow[shadowrow[i]][shadowcolumn[i]][shadowDepth[i]] = true;
	}

	return true;
}

pair<int, int> Board::getRotateTo(int row, int column)
{
	return make_pair(0 + column, 3 - row);
}

bool Board::canRotateBlock()
{
	for (int i = 0; i < 4; i++) {
		pair<int, int> rc = getRotateTo(currentRow[i] - currentPointRow, currentColumn[i] - currentPointColumn);
		if (rc.first + currentPointRow<0 || rc.first + currentPointRow >= 20
			|| rc.second + currentPointColumn<0 || rc.second + currentPointColumn >= 10
			|| board[rc.first + currentPointRow][rc.second + currentPointColumn][currentDepth[i]]) return false;
	}
	return true;
}

void Board::rotateBlock()
{
	if (!canRotateBlock()) return;

	for (int i = 0; i < 4; i++) {
		boardCurrent[currentRow[i]][currentColumn[i]][currentDepth[i]] = false;
		pair<int, int> rc = getRotateTo(currentRow[i] - currentPointRow, currentColumn[i] - currentPointColumn);
		blocks[rc.first + currentPointRow][rc.second + currentPointColumn][currentDepth[i]] 
			= blocks[currentRow[i]][currentColumn[i]][currentDepth[i]];
		currentRow[i] = rc.first + currentPointRow;
		currentColumn[i] = rc.second + currentPointColumn;
	}

	for (int i = 0; i < 4; i++) {
		boardCurrent[currentRow[i]][currentColumn[i]][currentDepth[i]] = true;
		boardShadow[shadowrow[i]][shadowcolumn[i]][shadowDepth[i]] = false;
	}

	shadowrow.clear();
	shadowcolumn.clear();
	shadowDepth.clear();
	for (int i = 0; i < 4; i++) {
		shadowrow.push_back(currentRow[i]);
		shadowcolumn.push_back(currentColumn[i]);
		shadowDepth.push_back(currentDepth[i]);
		boardShadow[shadowrow[i]][shadowcolumn[i]][shadowDepth[i]] = true;
	}
	while (movedownShadow());

}

void Board::removeCurrent() {
		for (int i = 0; i < 4; i++) {
			boardCurrent[currentRow[i]][currentColumn[i]][currentDepth[i]] = false;
			boardShadow[shadowrow[i]][shadowcolumn[i]][currentDepth[i]] = false;
		}
		currentRow.clear();
		currentColumn.clear();
		currentDepth.clear();
		shadowrow.clear();
		shadowcolumn.clear();
		shadowDepth.clear();
}

bool Board::movedown()
{
	if (!canmoveblock(1, 0, 0)) {
		for (int i = 0; i < 4; i++) {
			board[currentRow[i]][currentColumn[i]][currentDepth[i]] = true;
			boardCurrent[currentRow[i]][currentColumn[i]][currentDepth[i]] = false;
			boardShadow[shadowrow[i]][shadowcolumn[i]][currentDepth[i]] = false;
		}
		currentRow.clear();
		currentColumn.clear();
		currentDepth.clear();
		shadowrow.clear();
		shadowcolumn.clear();
		shadowDepth.clear();
		return false;
	}
	int k = 0;
	for (int i = ROW-1; i >= 0; i--) {
		for (int j = 0; j < COLUMN; j++) {
			for (int l = currentDepth[0]; l < currentDepth[0] + 1; l++) {
				if (boardCurrent[i][j][l]) {
					boardCurrent[i][j][l] = false;
					boardCurrent[i + 1][j][l] = true;
					blocks[i + 1][j][l] = blocks[i][j][l];
					k++;
					if (k >= 4) {
						currentPointRow += 1;
						for (int i = 0; i < 4; i++) currentRow[i] += 1;
						/*for (int i = 0; i < ROW; i++) {
								for (int k = 0; k < DEPTH; k++) {
									for (int j = 0; j < COLUMN; j++) {
										if (board[i][j][k] || boardCurrent[i][j][k])cout << 1 << " ";
										else cout << 0 << " ";
									}
									cout << " ";
								}
								cout << endl;
							}
							cout << endl;*/
						return true;
					}
				}
			}
		}
	}

	return true;
}

void Board::update()
{
	bool all;
	for (int i = 0; i < ROW; i++) {
		all = true;
		for (int j = 0; j < COLUMN; j++) for (int k = 0; k < DEPTH; k++) {
			if (!board[i][j][k]) {
				all = false;
				break;
			}
		}
		if (all) {
			for (int j = 0; j < COLUMN; j++) {
				for (int l = 0; l < DEPTH; l++) {
					board[i][j][l] = false;
					for (int k = i - 1; k >= 0; k--) {
						if (board[k][j][l]) {
							board[k][j][l] = false;
							board[k + 1][j][l] = true;
							blocks[k + 1][j][l] = blocks[k][j][l];
						}
						else {
							board[k + 1][j][l] = false;
						}
					}
				}
			}

		}
	}
}

void Board::clean()
{
	for (int i = 0; i < ROW; i++)for (int j = 0; j < COLUMN; j++)for (int k = 0; k < DEPTH; k++) {
		board[i][j][k] = false;
		boardCurrent[i][j][k] = false;
		boardShadow[i][j][k] = false;
	}
	shadowrow.clear();
	shadowcolumn.clear();
	shadowDepth.clear();
	currentRow.clear();
	currentColumn.clear();
	currentDepth.clear();
}
