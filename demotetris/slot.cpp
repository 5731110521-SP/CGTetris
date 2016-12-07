#include <vector>
#include "slot.h"

using namespace std;

Slot::Slot() {
	for (int i = 0; i < DIM; i++) for (int j = 0; j < DIM; j++) {
		nextblock[i][j] = false;
		shiftblock[i][j] = false;
	}
}

void Slot::addNext(Block b, int row, int column) {
	nextblock[row][column] = true;
	nextblocks[row][column] = b;
}

void Slot::addShift(Block b, int row, int column) {
	shiftblock[row][column] = true;
	shiftblocks[row][column] = b;
}
void Slot::clearNext() {
	for (int i = 0; i < DIM; i++) for (int j = 0; j < DIM; j++) {
		nextblock[i][j] = false;
	}
}
void Slot::clearShift() {
	for (int i = 0; i < DIM; i++) for (int j = 0; j < DIM; j++) {
		shiftblock[i][j] = false;
	}
}
bool Slot::isShiftEmpty() {
	for (int i = 0; i < DIM; i++) for (int j = 0; j < DIM; j++) {
		if (shiftblock[i][j]) return false;
	} return true;
}