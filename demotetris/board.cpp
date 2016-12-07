#include <vector>
#include "board.h"
#include "block.h"
#include "vec3f.h"

using namespace std;

Board::Board() {
	for (int i = 0; i < 20; i++) for (int j = 0; j < 10; j++) {
		board[i][j] = false;
		boardCurrent[i][j] = false;
		boardShadow[i][j] = false;
	}
	/*Block block = Block();
	addblocks(block, 4, 0);
	addblocks(block, 5, 0);
	addblocks(block, 5, 1);
	addblocks(block, 6, 1);*/
}

void Board::addblocks(Block b, int row, int column)
{
	boardCurrent[row][column] = true;
	blocks[row][column] = b;
	currentRow.push_back(row);
	currentColumn.push_back(column);

	boardShadow[row][column]=true;
	shadowrow.push_back(row);
	shadowcolumn.push_back(column);

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
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) cout << board[i][j] << " ";
		cout << endl;
	}
	cout << endl;
	*/
}

bool Board::onCurrent(int row, int column)
{
	/*for (int i = 0; i < 4; i++) {
	if (currentRow[i] == row && currentColumn[i] == column)return true;
	}*/
	return boardCurrent[row][column];
	//return false;
}

bool Board::canmoveblock(int row, int column)
{
	for (int i = 0; i < 4; i++) {
		//if (board[currentRow[i] + row][currentColumn[i] + column] && !onCurrent(currentRow[i] + row, currentColumn[i] + column)) return false;
		if (currentRow[i] + row<0 || currentRow[i] + row >= 20
			|| currentColumn[i] + column<0 || currentColumn[i] + column >= 10
			|| board[currentRow[i] + row][currentColumn[i] + column]) return false;
	}
	return true;
}

void Board::moveblock(int column) //left=-1 right=1
{
	if (!canmoveblock(0, column)) return;
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
		for (int j = 0; j < 10; j++) {
			if (k >= 4)break;
			for (int i = 0; i < 20; i++) {
				if (boardCurrent[i][j]) {
					boardCurrent[i][j] = false;
					boardCurrent[i][j - 1] = true;
					blocks[i][j - 1] = blocks[i][j];
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
	else {
		int k = 0;
		for (int j = 9; j >= 0; j--) {
			if (k >= 4)break;
			for (int i = 0; i < 20; i++) {
				if (boardCurrent[i][j]) {
					boardCurrent[i][j] = false;
					boardCurrent[i][j + 1] = true;
					blocks[i][j + 1] = blocks[i][j];
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
	for (int i = 0; i < 4; i++) {
		boardShadow[shadowrow[i]][shadowcolumn[i]] = false;
	}
	shadowrow.clear();
	shadowcolumn.clear();
	for (int i = 0; i < 4; i++) {
		shadowrow.push_back(currentRow[i]);
		shadowcolumn.push_back(currentColumn[i]);
		boardShadow[shadowrow[i]][shadowcolumn[i]] = true;
	}
	while (movedownShadow());
}

bool Board::canmoveShadow(int row, int column)
{
	for (int i = 0; i < 4; i++) {
		if (shadowrow[i] + row<0 || shadowrow[i] + row >= 20
			|| shadowcolumn[i] + column<0 || shadowcolumn[i] + column >= 10
			|| board[shadowrow[i] + row][shadowcolumn[i] + column]) return false;
	}
	return true;
}

bool Board::movedownShadow()
{
	if (!canmoveShadow(1, 0)) {
		return false;
	}
	int k = 0;
	for (int i = 19; i >= 0; i--) {
		for (int j = 0; j < 10; j++) {
			if (boardShadow[i][j]) {
				boardShadow[i][j] = false;
				boardShadow[i + 1][j] = true;
				k++;
				if (k >= 4) {
					for (int i = 0; i < 4; i++) shadowrow[i] += 1;
					return true;
				}
			}
		}
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
			|| board[rc.first + currentPointRow][rc.second + currentPointColumn]) return false;
	}
	return true;
}

void Board::rotateBlock()
{
	if (!canRotateBlock()) return;

	for (int i = 0; i < 4; i++) {
		boardCurrent[currentRow[i]][currentColumn[i]] = false;
		pair<int, int> rc = getRotateTo(currentRow[i] - currentPointRow, currentColumn[i] - currentPointColumn);
		blocks[rc.first + currentPointRow][rc.second + currentPointColumn] = blocks[currentRow[i]][currentColumn[i]];
		currentRow[i] = rc.first + currentPointRow;
		currentColumn[i] = rc.second + currentPointColumn;
	}

	for (int i = 0; i < 4; i++) {
		boardCurrent[currentRow[i]][currentColumn[i]] = true;
		boardShadow[shadowrow[i]][shadowcolumn[i]] = false;
	}

	shadowrow.clear();
	shadowcolumn.clear();
	for (int i = 0; i < 4; i++) {
		shadowrow.push_back(currentRow[i]);
		shadowcolumn.push_back(currentColumn[i]);
		boardShadow[shadowrow[i]][shadowcolumn[i]] = true;
	}
	while (movedownShadow());

}

bool Board::movedown()
{
	if (!canmoveblock(1, 0)) {
		for (int i = 0; i < 4; i++) {
			board[currentRow[i]][currentColumn[i]] = true;
			boardCurrent[currentRow[i]][currentColumn[i]] = false;
			boardShadow[shadowrow[i]][shadowcolumn[i]] = false;
		}
		currentRow.clear();
		currentColumn.clear();
		shadowrow.clear();
		shadowcolumn.clear();
		return false;
	}
	int k = 0;
	for (int i = 19; i >= 0; i--) {
		for (int j = 0; j < 10; j++) {
			if (boardCurrent[i][j]) {
				boardCurrent[i][j] = false;
				boardCurrent[i + 1][j] = true;
				blocks[i + 1][j] = blocks[i][j];
				k++;
				if (k >= 4) {
					currentPointRow += 1;
					for (int i = 0; i < 4; i++) currentRow[i] += 1;
					return true;
				}
			}
		}
	}

	return true;
}

void Board::update()
{
	bool all;
	for (int i = 0; i < 20; i++) {
		all = true;
		for (int j = 0; j < 10; j++) {
			if (!board[i][j]) {
				all = false;
				break;
			}
		}
		if (all) {
			for (int j = 0; j < 10; j++) {
				board[i][j] = false;
				for (int k = i - 1; k >= 0; k--) {
					if (board[k][j]) {
						board[k][j] = false;
						board[k + 1][j] = true;
						blocks[k + 1][j] = blocks[k][j];
					}
					else {
						board[k + 1][j] = false;
					}
				}
			}

		}
	}
}
