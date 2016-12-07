#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <set>
#include <map>
#include <stdlib.h>
#include <vector>
#include <time.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "modelloader.h"
#include "vec3f.h"
#include "block.h"
#include "board.h"
#include "slot.h"

#define BOARD_X 5
#define BOARD_Y 17
#define HOME_X 5
#define HOME_Y 77
#define NEXT_X 18
#define NEXT_Y 17

using namespace std;

Board board;
Slot slot;
int choose;
int current=0;
int prevChoose;
int random;
int prevType,nextType,keepCurrent,keep=-1;
bool pressedS = false,start=false,pause=false;
vector<int> typeBlock = { 0,1,2,3,4,5,6,7 };
//int typeBlock[] = {0,1,1,2,3,3,4,4,5,5,6,7};
vector<Block> _blocks;
Model_OBJ model;
Model_OBJ number[10],num[10], wire;

int isStart=0;
GLuint _textureId;
GLfloat red[] = { 1,0,0 };
GLfloat green[] = { 0,1,0 };
GLfloat blue[] = { 0,0,1 };
GLfloat yellow[] = { 0.95,0.8,0.16 };
GLfloat pink[] = { 1,0,1 };
GLfloat purple[] = { 0.5,0,0.5 };
GLfloat brown[] = { 0.62,0.38,0.17 };
GLfloat gray[] = { 0.7,0.7,0.7 };
GLfloat white[] = { 1,1,1 };
GLfloat shadow[] = { 0.4,0.4,0.4 };
map<int,GLfloat*> col;
map<char,Model_OBJ> alphabet;
int speed=1000; bool space=false;
// Camera position
float x = 0.0, z = -5.0,dx,dy=-60,dz; // initially 5 units south of origin
float deltaMove = 0.0; // initially camera doesn't move

// Camera direction
float lx = 0.0, lz = 1.0; // camera points initially along y-axis
float angle = 0.0; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts

void randomBlock() {
	srand(time(0));
	random_shuffle(typeBlock.begin(), typeBlock.end());
	random = rand() % typeBlock.size();
	if (typeBlock[random] != choose) choose = typeBlock[random];
	else choose = typeBlock[random / 2];
}

Block block, nextblock,keepBlock;
void createBlock(int oldChoose) {

	//----------------------------------------------------------------------
	slot.clearNext();
	if (keep != 1) {
		prevChoose = oldChoose;
		block = nextblock;
		randomBlock();
		nextblock = Block(col[choose]);
	}
	else if(keep==1 && !slot.isShiftEmpty()){
		keep = -1;

	}
	//----------------------------------------------------------------------
	if (oldChoose == 0) { // ---
		board.addblocks(block, 1, 0 ,0);
		board.addblocks(block, 1, 1 ,0);
		board.addblocks(block, 1, 2 ,0);
		board.addblocks(block, 1, 3 ,0);
		//-----------------------------------
	}
	else if (oldChoose == 1) {				// -
		board.addblocks(block, 2, 1,0);	// ---
		board.addblocks(block, 2, 2,0);
		board.addblocks(block, 2, 3,0);
		board.addblocks(block, 1, 1,0);
		//-----------------------------------
	}
	else if (oldChoose == 2) {				// --
		board.addblocks(block, 1, 1,0);	// --
		board.addblocks(block, 1, 2,0);
		board.addblocks(block, 2, 1,0);
		board.addblocks(block, 2, 2,0);
	}
	else if (oldChoose == 3) {				//  -
		board.addblocks(block, 1, 1,0);	// ---
		board.addblocks(block, 1, 2,0);
		board.addblocks(block, 1, 3,0);
		board.addblocks(block, 2, 2,0);
	}
	else if (oldChoose == 4) {				//  --
		board.addblocks(block, 2, 2,0);	//--
		board.addblocks(block, 2, 3,0);
		board.addblocks(block, 1, 1,0);
		board.addblocks(block, 1, 2,0);
	}
	else if (oldChoose == 5) {				//--
		board.addblocks(block, 2, 1,0);	//  --
		board.addblocks(block, 2, 2,0);
		board.addblocks(block, 1, 2,0);
		board.addblocks(block, 1, 3,0);
	}
	else if(oldChoose == 6){
		board.addblocks(block, 2, 2,0);	//  -
		board.addblocks(block, 2, 3,0);	//---
		board.addblocks(block, 2, 4,0);
		board.addblocks(block, 1, 4,0);
	}
	else {
		board.addblocks(block, 1, 1,0);	//  -
		board.addblocks(block, 2, 1,0);	//  -
		board.addblocks(block, 3, 1,0);	//  -
		board.addblocks(block, 4, 1,0);	//  -
	}

	if (choose == 0) { // ---

		slot.addNext(nextblock, 1, 0);
		slot.addNext(nextblock, 1, 1);
		slot.addNext(nextblock, 1, 2);
		slot.addNext(nextblock, 1, 3);
		//-----------------------------------

	}
	else if (choose == 1) {				// -

		slot.addNext(nextblock, 2, 0);
		slot.addNext(nextblock, 2, 1);
		slot.addNext(nextblock, 2, 2);
		slot.addNext(nextblock, 1, 0);

	}
	else if (choose == 2) {				// --

		slot.addNext(nextblock, 1, 1);
		slot.addNext(nextblock, 1, 2);
		slot.addNext(nextblock, 2, 1);
		slot.addNext(nextblock, 2, 2);

	}
	else if (choose == 3) {				//  -

		slot.addNext(nextblock, 1, 0);	// ---
		slot.addNext(nextblock, 1, 1);
		slot.addNext(nextblock, 1, 2);
		slot.addNext(nextblock, 2, 1);

	}
	else if (choose == 4) {				//  --

		slot.addNext(nextblock, 2, 1);	//--
		slot.addNext(nextblock, 2, 2);
		slot.addNext(nextblock, 1, 0);
		slot.addNext(nextblock, 1, 1);

	}
	else if (choose == 5) {				//--

		slot.addNext(nextblock, 2, 0);	//  --
		slot.addNext(nextblock, 2, 1);
		slot.addNext(nextblock, 1, 1);
		slot.addNext(nextblock, 1, 2);

	}
	else if (choose == 6) {
		slot.addNext(nextblock, 2, 1);	//  -
		slot.addNext(nextblock, 2, 2);	//---
		slot.addNext(nextblock, 2, 3);
		slot.addNext(nextblock, 1, 3);
	}
	else {
		slot.addNext(nextblock, 1, 1);
		slot.addNext(nextblock, 2, 1);
		slot.addNext(nextblock, 3, 1);
		slot.addNext(nextblock, 4, 1);
	}

	/*
	if (keepCurrent == 0) { // ---

	slot.addShift(keepBlock, 1, 0);
	slot.addShift(keepBlock, 1, 1);
	slot.addShift(keepBlock, 1, 2);
	slot.addShift(keepBlock, 1, 3);
	//-----------------------------------

	}
	else if (keepCurrent == 1) {				// -

	slot.addShift(keepBlock, 2, 0);
	slot.addShift(keepBlock, 2, 1);
	slot.addShift(keepBlock, 2, 2);
	slot.addShift(keepBlock, 1, 0);

	}
	else if (keepCurrent == 2) {				// --

	slot.addShift(keepBlock, 1, 1);
	slot.addShift(keepBlock, 1, 2);
	slot.addShift(keepBlock, 2, 1);
	slot.addShift(keepBlock, 2, 2);

	}
	else if (keepCurrent == 3) {				//  -

	slot.addShift(keepBlock, 1, 0);	// ---
	slot.addShift(keepBlock, 1, 1);
	slot.addShift(keepBlock, 1, 2);
	slot.addShift(keepBlock, 2, 1);

	}
	else if (keepCurrent == 4) {				//  --

	slot.addShift(keepBlock, 2, 1);	//--
	slot.addShift(keepBlock, 2, 2);
	slot.addShift(keepBlock, 1, 0);
	slot.addShift(keepBlock, 1, 1);

	}
	else if (keepCurrent == 5) {				//--

	slot.addShift(keepBlock, 2, 0);	//  --
	slot.addShift(keepBlock, 2, 1);
	slot.addShift(keepBlock, 1, 1);
	slot.addShift(keepBlock, 1, 2);

	}
	else if (keepCurrent == 6) {
	slot.addShift(keepBlock, 2, 1);	//  -
	slot.addShift(keepBlock, 2, 2);	//---
	slot.addShift(keepBlock, 2, 3);
	slot.addShift(keepBlock, 1, 3);
	}
	else {
	slot.addShift(keepBlock, 1, 1);
	slot.addShift(keepBlock, 2, 1);
	slot.addShift(keepBlock, 3, 1);
	slot.addShift(keepBlock, 4, 1);
	}*/


	glutPostRedisplay();
}
void handleArrow(int key,int x,int y) {
	if (!pause && key == GLUT_KEY_LEFT) {
		if (!board.currentColumn.empty()) {
			board.moveblock(-1,0);
			glutPostRedisplay();
		}
	}
	if (!pause && key == GLUT_KEY_DOWN) {
		if (!board.currentColumn.empty()) {
			if (!board.movedown()) {
				createBlock(choose);
			}

			board.update();
			glutPostRedisplay();
		}
	}
	if (!pause && key == GLUT_KEY_RIGHT) {
		if (!board.currentColumn.empty()) {
			board.moveblock(1,0);
			glutPostRedisplay();
		}
	}
	if (!pause && key == GLUT_KEY_UP) {
		if (!board.currentColumn.empty()) {
			board.rotateBlock();
			glutPostRedisplay();
		}
	}
}
void releaseSpecialKey(unsigned char key, int x, int y){
	if (key == '=') deltaMove = 0.0;
	if (key == '-') deltaMove = 0.0;
}
void handleKeypress(unsigned char key,int x,int y) {
    if (key == 27) exit(0);
    if (key == 32 && isStart == 1) space=true;
//    if (key == '-') deltaMove = 1.0;
//	if (key == '=') deltaMove = -1.0;
	/*if (key == 's') {
        if(!pressedS){
            pressedS = true;
			randomBlock();
			nextblock = Block(col[choose]);

            createBlock(choose);
            glutPostRedisplay();
        }
	}*/
	if (key == 'k') {
		if (!board.currentColumn.empty()) {
			board.removeCurrent();
			if (slot.isShiftEmpty()) {
				keepCurrent = prevChoose;
				keepBlock = block;
				for (int i = 0; i < DIM; i++) for (int j = 0; j < DIM; j++) {
					slot.shiftblock[i][j] = true;
				}
				createBlock(choose);
			}
			else if (!slot.isShiftEmpty()) {
				Block tmp = block;
				block = keepBlock;
				keepBlock = tmp;
				int t = keepCurrent;
				cout << "kc = " << t << endl;
				cout << "pv = " << prevChoose << endl;
				keepCurrent = prevChoose;
				prevChoose = t;
				keep = 1;
				createBlock(prevChoose);
			}
		}
	}
	if (key == 'a') {
		if (!board.currentColumn.empty()) {
			board.moveblock(0, 1);
			glutPostRedisplay();
		}
	}
	if (key == 'z') {
		if (!board.currentColumn.empty()) {
			board.moveblock(0, -1);
			glutPostRedisplay();
		}
	}
	if (key == '\/') {
		keep = 1;
	}
	if (key == 's') {
		if (!start&&isStart==0) {
			isStart = 1;
			start = true;
		}
	}
	if (key == 'p') {
        pause = !pause;
	}
}
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) { // left mouse button pressed
			isDragging = 1; // start dragging
			xDragStart = x; // save x where button first pressed
		}
		else  { /* (state = GLUT_UP) */
			angle += deltaAngle; // update camera turning angle
			isDragging = 0; // no longer dragging
		}
	}
}
void mouseMove(int x, int y) {
    if (isDragging) { // only when dragging
		// update the change in angle
		deltaAngle = (x - xDragStart) * 0.005;

		// camera's direction is set to angle + deltaAngle
		lx = -sin(angle + deltaAngle);
		lz = cos(angle + deltaAngle);
	}
}
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
											 //Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
						  //as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	col[0]=red,col[1]=green,col[2]=blue,col[3]=yellow,
    col[4]=pink,col[5]=purple,col[6]=brown,col[7]=gray;
}
void DrawNext() {
	Block b = nextblock;
	for (int i = 0; i < DIM; i++) for(int j=0; j<DIM; j++)  {
		if (slot.nextblock[i][j]) {
			glPushMatrix();
			Block b = slot.nextblocks[i][j];
			glTranslatef(j * 2 + NEXT_X, -i * 2 + NEXT_Y, 0);    //calculate location
			glColor3f(b.color[0], b.color[1], b.color[2]);
			model.Draw();
			glPopMatrix();
		}
	}
	for(int i=0; i<4; i++) {
        glPushMatrix();
        glColor3fv(white);
        glTranslatef(i*2+NEXT_X,NEXT_Y-10,0);
        if(i==0) alphabet['n'].Draw();
        else if(i==1) alphabet['e'].Draw();
        else if(i==2) alphabet['x'].Draw();
        else if(i==3) alphabet['t'].Draw();
        glPopMatrix();
	}
}
void DrawBorder(GLfloat *color,char *type) {
	for (int i = 0; i <= ROW; i++)
		for (int j = 0; j <= COLUMN; j++)
			for (int k = 0; k <= DEPTH; k++) {

		glPushMatrix();
		//Block b = Block(white);
		//if(i==0) glTranslatef(j*2-7,19,0);
		//else if(i==21) glTranslatef(j*2-7,-23,0);
	   // else if(j==0) glTranslatef(-7,-i*2+19,0);
	   // else if(j==11) glTranslatef(15,-i*2+19,0);
//                b.drawCube('t');
		//glColor3f(b.color[0],b.color[1],b.color[2]);
		//model.Draw();

		glLineWidth(0.5);
		glColor3f(gray[0], gray[1], gray[2]);
		glBegin(GL_LINES);
		glVertex3f(-7 + 1, 19 - 1 - i * 2, -k * 2 + 1);
		glVertex3f(-7 + 1 + COLUMN * 2, 19 - 1 - i * 2, -k * 2 + 1);
		glVertex3f(-7 + 1 + j * 2, 19 - 1, -k * 2 + 1);
		glVertex3f(-7 + 1 + j * 2, 19 - 1 - ROW * 2, -k * 2 + 1);
		glVertex3f(-7 + 1 + j * 2, 19 - 1 - i * 2, 1);
		glVertex3f(-7 + 1 + j * 2, 19 - 1 - i * 2, -DEPTH * 2 + 1);
		glEnd();
		glPopMatrix();
	}
    /*glBegin(GL_QUADS);

    glColor3f(1,1,1);
    glVertex3f(-6,18,-1);
    glVertex3f(14,18,-1);
    glVertex3f(14,-22,-1);
    glVertex3f(-6,-22,-1);
    glEnd();
    */

    int borderx,bordery,lower_frame;
    Model_OBJ m;
    if(type=="game") { borderx=BOARD_X; bordery=BOARD_Y; m=model; lower_frame=-bordery-6; }
    else if(type=="home") { borderx=HOME_X; bordery=HOME_Y+1; m=wire; lower_frame=-BOARD_Y-6+60+1; }
    //block border
	if (type == "home") {
		for (int i = 0; i <= 21; i++) for (int j = 0; j <= 11; j++) {
			if (i == 0 || i == 21 || j == 0 || j == 11) {
				glPushMatrix();
				Block b = Block(color);
				if (i == 0) glTranslatef(j * 2 - borderx - 2, bordery + 2, 0);
				else if (i == 21) glTranslatef(j * 2 - borderx - 2, lower_frame, 0);
				else if (j == 0) glTranslatef(-borderx - 2, -i * 2 + bordery + 2, 0);
				else if (j == 11) glTranslatef(borderx + 10, -i * 2 + bordery + 2, 0);
				glColor3f(b.color[0], b.color[1], b.color[2]);
				//            b.drawCube('t');
				model.Draw();
				glPopMatrix();
			}
		}
	}

}
void DrawGameBoard() {
    DrawBorder(white,"game");
	DrawNext();
    int k = 0;
    for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			for (int dep = 0; dep < DEPTH; dep++) {
				//cout << h << " " << i << " " << j << " " << dep << endl;
				if (board.board[i][j][dep] || board.boardCurrent[i][j][dep]) {
					//h++;

				//if (board.board[i][j][0] || board.boardCurrent[i][j][0]) {
					glPushMatrix();
					Block b = board.blocks[i][j][dep];
					//Block b = board.blocks[i][j][0];
					//glTranslatef(j * 2 - 5, -i * 2 + 17, 0);    //calculate location
					glTranslatef(j * 2 - BOARD_X, -i * 2 + BOARD_Y, -dep * 2);    //calculate location
	//				b.drawCube('c');
					glColor3f(b.color[0], b.color[1], b.color[2]);
					model.Draw();
					glPopMatrix();
				}
				else if (board.boardShadow[i][j][dep]) {
				//else if (board.boardShadow[i][j][0]) {
					glPushMatrix();
					Block b = Block(shadow);
					//glTranslatef(j * 2 - 5, -i * 2 + 17, 0);    //calculate location
					glTranslatef(j * 2 - BOARD_X, -i * 2 + BOARD_Y, -dep * 2);    //calculate location
	//				b.drawCube('c');
					glColor3f(b.color[0], b.color[1], b.color[2]);
					model.Draw();
					glPopMatrix();
				}
                else if (pause) {
                    glPushMatrix();
                    glColor3fv(white);
                    glTranslatef(j*2-BOARD_X,-i*2+BOARD_Y, 0);    //calculate location
                    glPushMatrix();
                    glScalef(1.7,1.7,1.7);
                    if (i==8&&j==3) alphabet['p'].Draw();
                    else if (i==8&&j==4) alphabet['a'].Draw();
                    else if (i==8&&j==5) alphabet['u'].Draw();
                    else if (i==8&&j==6) alphabet['s'].Draw();
                    else if (i==8&&j==7) alphabet['e'].Draw();
                    glPopMatrix();
                    glPopMatrix();
                }
			}
		}
	}
}
void DrawHome() {
    DrawBorder(white,"home");
    for(int i=0; i<20; i++) {
        for(int j=0; j<10; j++) {
            glPushMatrix();
            glTranslatef(j*2-HOME_X,-i*2+HOME_Y+1,0);
            glColor3fv(purple);
            //uncomment if want purple cube background
//            model.Draw();
            glPopMatrix();
        //----------------------------------------------------
            glPushMatrix();
            glTranslatef(j*2-HOME_X,-i*2+HOME_Y+1,0+1);
            glColor3fv(white);
            //----------------------------------------------------
            glPushMatrix();
            glScalef(1.5,1.5,1.5);
            glColor3fv(yellow);
            if (i==3&&j==2) alphabet['t'].Draw();
            else if (i==3&&j==3) alphabet['e'].Draw();
            else if (i==3&&j==4) alphabet['t'].Draw();
            else if (i==3&&j==5) alphabet['r'].Draw();
            else if (i==3&&j==6) alphabet['i'].Draw();
            else if (i==3&&j==7) alphabet['s'].Draw();
            glPopMatrix();
            //----------------------------------------------------
            glColor3fv(white);
            if (i==14&&j==2) alphabet['s'].Draw();
            else if (i==14&&j==3) alphabet['t'].Draw();
            else if (i==14&&j==4) alphabet['a'].Draw();
            else if (i==14&&j==5) alphabet['r'].Draw();
            else if (i==14&&j==6) alphabet['t'].Draw();
            else if (i==15&&j==7) alphabet['s'].Draw();
            //----------------------------------------------------
            glPushMatrix();
            glScalef(0.5,0.5,0.5);
            if (i==15&&j==2) alphabet['p'].Draw();
            else if (i==15&&j==3) alphabet['r'].Draw();
            else if (i==15&&j==4) alphabet['e'].Draw();
            else if (i==15&&j==5) alphabet['s'].Draw();
            else if (i==15&&j==6) alphabet['s'].Draw();
            glPopMatrix();
            //----------------------------------------------------
            glColor3fv(red);
            if (i==7&&j==1) model.Draw();
            else if (i==8&&j==1) model.Draw();
            else if (i==9&&j==1) model.Draw();
            else if (i==10&&j==1) model.Draw();
            glColor3fv(green);
            if (i==8&&j==4) model.Draw();
            else if (i==9&&j==3) model.Draw();
            else if (i==9&&j==4) model.Draw();
            else if (i==9&&j==5) model.Draw();
            glColor3fv(blue);
            if (i==8&&j==7) model.Draw();
            else if (i==8&&j==8) model.Draw();
            else if (i==9&&j==7) model.Draw();
            else if (i==9&&j==8) model.Draw();
            glPopMatrix();
        //----------------------------------------------------

        }
    }
}
void handleResize(int w, int h) {
    	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
								 //Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,                  //The camera angle
		(double)w / (double)h, //The width-to-height ratio
		1.0,                   //The near z clipping coordinate
		200.0);                //The far z clipping coordinate
}
void setUptexture(char *file) {
    Image* image = loadBMP(file);
	_textureId = loadTexture(image);
	delete image;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
void setUplighting() {
	//lighting
	GLfloat ambientColor[] = { 0.2f,0.2f,0.2f,1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
														//**(0.0)Positioned at (4, 0, 8)
	GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	//Add directed light
	GLfloat lightColor1[] = { 0.4f, 0.2f, 0.3f, 1.0f }; //Color (0.5, 0.2, 0.2)
														//**(1.0)Coming from the direction (-1, 0.5, 0.5)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
}
void showScore() {

	glScalef(0.7, 0.7, 0.7);
	Block bb = Block(white);
	glColor3f(bb.color[0], bb.color[1], bb.color[2]);

	glPushMatrix();
	glTranslatef(28 , 28, 0);
	num[board.digit1].Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(34, 28, 0);
	num[board.digit2].Draw();
	glPopMatrix();

}
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	setUplighting();
	glTranslatef(0+dx,0+dy,dz-68);
//	setUptexture("D:/_fang/year 3/cg/demotetris/texture/brick.bmp");
//	setUptexture("texture/brick.bmp");

    gluLookAt(
		x, 1, z,               //camera location @ (x,1,z)
		x - lx, 1, z - lz,               // looking along vector (lx,1,lz)
		0, 1, 0);              // with up vector (0,1,0)

	DrawGameBoard();
	DrawHome();
	DrawNext();

	if(!start) showScore();

	glutSwapBuffers();
}
void updatecam(void) {
	if (deltaMove) { // update camera position
		x += deltaMove * lx * 0.01;
		z += deltaMove * lz * 0.01;
	}
	glutPostRedisplay(); // redisplay everything
}
void updateframe(int value) {
    if (start) {
        dy+=0.5;
        if(dy>=0) {
            start=false;
            pressedS = true;
			randomBlock();
			nextblock = Block(col[choose]);
            createBlock(choose);
            glutPostRedisplay();
        }
    }
    glutPostRedisplay();
    glutTimerFunc(10, updateframe, 0);
}
void update(int value) {
	if (!pause&&!board.currentColumn.empty()) {
        speed=(space)?10:1000;
		if (!board.movedown()) {
            space=false;
            createBlock(choose);
			board.update();
		}


		glutPostRedisplay();
	}
	glutTimerFunc(speed, update, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("TETRIS ><");
	initRendering();

	glutReshapeFunc(handleResize);
	glutDisplayFunc(drawScene);
	glutIdleFunc(updatecam);

	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleArrow);
//	glutKeyboardUpFunc(releaseSpecialKey);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	model.Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/cube-m.obj");
//	model.Load("model/cube-m.obj");

//	/*
    number[0].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/0-triang-m.obj");
    number[1].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/1-triang-m.obj");
    number[2].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/2-triang-m.obj");
    number[3].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/3-triang-m.obj");
    number[4].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/4-triang-m.obj");
    number[5].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/5-triang-m.obj");
    number[6].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/6-triang-m.obj");
    number[7].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/7-triang-m.obj");
    number[8].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/8-triang-m.obj");
    number[9].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/9-triang-m.obj");
//	*/
//	number[0].Load("model/character_obj/0-triang-m.obj");
//	number[1].Load("model/character_obj/1-triang-m.obj");
//	number[2].Load("model/character_obj/2-triang-m.obj");
//	number[3].Load("model/character_obj/3-triang-m.obj");
//	number[4].Load("model/character_obj/4-triang-m.obj");
//	number[5].Load("model/character_obj/5-triang-m.obj");
//	number[6].Load("model/character_obj/6-triang-m.obj");
//	number[7].Load("model/character_obj/7-triang-m.obj");
//	number[8].Load("model/character_obj/8-triang-m.obj");
//	number[9].Load("model/character_obj/9-triang-m.obj");
//
//	alphabet['a'].Load("model/character_obj/a-triang-m.obj");
//	alphabet['c'].Load("model/character_obj/c-triang-m.obj");
//	alphabet['e'].Load("model/character_obj/e-triang-m.obj");
//	alphabet['i'].Load("model/character_obj/i-triang-m.obj");
//	alphabet['k'].Load("model/character_obj/k-triang-m.obj");
//	alphabet['n'].Load("model/character_obj/n-triang-m.obj");
//	alphabet['o'].Load("model/character_obj/o-triang-m.obj");
//	alphabet['p'].Load("model/character_obj/p-triang-m.obj");
//	alphabet['r'].Load("model/character_obj/r-triang-m.obj");
//	alphabet['s'].Load("model/character_obj/s-triang-m.obj");
//	alphabet['t'].Load("model/character_obj/t-triang-m.obj");
//	alphabet['u'].Load("model/character_obj/u-triang-m.obj");
//	alphabet['x'].Load("model/character_obj/x-triang-m.obj");
//	/*
	alphabet['a'].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/a-triang-m.obj");
	alphabet['c'].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/c-triang-m.obj");
	alphabet['e'].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/e-triang-m.obj");
	alphabet['i'].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/i-triang-m.obj");
	alphabet['k'].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/k-triang-m.obj");
	alphabet['n'].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/n-triang-m.obj");
	alphabet['o'].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/o-triang-m.obj");
	alphabet['p'].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/p-triang-m.obj");
	alphabet['r'].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/r-triang-m.obj");
	alphabet['s'].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/s-triang-m.obj");
	alphabet['t'].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/t-triang-m.obj");
	alphabet['u'].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/u-triang-m.obj");
	alphabet['x'].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/character_obj/x-triang-m.obj");
//	*/

//	num[0].Load("model/0.obj");
//	num[1].Load("model/1.obj");
//	num[2].Load("model/2.obj");
//	num[3].Load("model/3.obj");
//	num[4].Load("model/4.obj");
//	num[5].Load("model/5.obj");
//	num[6].Load("model/6.obj");
//	num[7].Load("model/7.obj");
//	num[8].Load("model/8.obj");
//	num[9].Load("model/9.obj");

	glutTimerFunc(speed, update, 0);
	glutTimerFunc(10, updateframe, 0);

	glutMainLoop();
	return 0;
}
