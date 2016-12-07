#ifndef SLOT_H_INCLUDED
#define SLOT_H_INCLUDED

#include <GL/glut.h>
#include "block.h"
#define DIM 6
class Slot {
public:
	bool nextblock[DIM][DIM];
	bool shiftblock[DIM][DIM];
	Block nextblocks[DIM][DIM];
	Block shiftblocks[DIM][DIM];

	Slot();

	void addNext(Block b,int row,int col);
	void addShift(Block b,int row,int col);
	void clearNext();
	void clearShift();
	bool isShiftEmpty();
};

#endif
