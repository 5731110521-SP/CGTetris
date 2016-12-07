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
#define NEXT_X 19
#define NEXT_Y 17

using namespace std;

Board board;
Slot slot;
int choose;
int current=0;
int random;
int prevType,nextType,keep=0,keepCurrent;
bool pressedS = false,start=false,pause=false;
vector<int> typeBlock = { 0,1,2,3,4,5,6,7 };
//int typeBlock[] = {0,1,1,2,3,3,4,4,5,5,6,7};
Model_OBJ model,number[10];
GLuint _textureId;
GLfloat red[] = { 1,0,0 };
GLfloat green[] = { 0,1,0 };
GLfloat blue[] = { 0,0,1 };
GLfloat yellow[] = { 0.3,0.3,0 };
GLfloat pink[] = { 0.5,0,0.5 };
GLfloat purple[] = { 1,0,1 };
GLfloat brown[] = { 0,1,1 };
GLfloat gray[] = { 0.7,0.7,0.7 };
GLfloat white[] = { 1,1,1 };
GLfloat shadow[] = { 0.4,0.4,0.4 };
map<int,GLfloat*> col;
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
Block block, nextblock;
void createBlock(int oldChoose) {
	//----------------------------------------------------------------------
	slot.clearNext();
	block = nextblock;
	randomBlock();
	if (keep == 1 && !slot.isShiftEmpty()) {
		keep = 0;
		swap(keepCurrent, oldChoose);
		oldChoose = choose;
	}
	nextblock = Block(col[choose]);
	//----------------------------------------------------------------------
	if (oldChoose == 0) { // ---
		board.addblocks(block, 1, 3);
		board.addblocks(block, 1, 4);
		board.addblocks(block, 1, 5);
		board.addblocks(block, 1, 6);
		//-----------------------------------

		board.currentPointRow = 0;
		board.currentPointColumn = 3;
	}
	else if (oldChoose == 1) {				// -
		board.addblocks(block, 2, 1);	// ---
		board.addblocks(block, 2, 2);
		board.addblocks(block, 2, 3);
		board.addblocks(block, 1, 1);
		//-----------------------------------

		board.currentPointRow = 0;
		board.currentPointColumn = 0;
	}
	else if (oldChoose == 2) {				// --
		board.addblocks(block, 1, 1);	// --
		board.addblocks(block, 1, 2);
		board.addblocks(block, 2, 1);
		board.addblocks(block, 2, 2);
		//-----------------------------------------

		board.currentPointRow = 0;
		board.currentPointColumn = 0;
	}
	else if (oldChoose == 3) {				//  -
		board.addblocks(block, 1, 1);	// ---
		board.addblocks(block, 1, 2);
		board.addblocks(block, 1, 3);
		board.addblocks(block, 2, 2);
		//-----------------------------------------


		board.currentPointRow = 0;
		board.currentPointColumn = 0;
	}
	else if (oldChoose == 4) {				//  --
		board.addblocks(block, 2, 2);	//--
		board.addblocks(block, 2, 3);
		board.addblocks(block, 1, 1);
		board.addblocks(block, 1, 2);
		//------------------------------------


		board.currentPointRow = 0;
		board.currentPointColumn = 0;
	}
	else if (oldChoose == 5) {				//--
		board.addblocks(block, 2, 1);	//  --
		board.addblocks(block, 2, 2);
		board.addblocks(block, 1, 2);
		board.addblocks(block, 1, 3);
		//--------------------------------------


		board.currentPointRow = 0;
		board.currentPointColumn = 0;
	}
	else if(oldChoose == 6){
		board.addblocks(block, 2, 2);	//  -
		board.addblocks(block, 2, 3);	//---
		board.addblocks(block, 2, 4);
		board.addblocks(block, 1, 4);
		//------------------------------------


		board.currentPointRow = 0;
		board.currentPointColumn = 0;
	}
	else {
		board.addblocks(block, 1, 1);	//  -
		board.addblocks(block, 2, 1);	//  -
		board.addblocks(block, 3, 1);	//  -
		board.addblocks(block, 4, 1);	//  -
		//--------------------------------------

		board.currentPointRow = 1;
		board.currentPointColumn = 0;
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

	glutPostRedisplay();
}
void handleArrow(int key,int x,int y) {
	if (!pause && key == GLUT_KEY_LEFT) {
		if (!board.currentColumn.empty()) {
			board.moveblock(-1);
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
			board.moveblock(1);
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
    if (key == 32) space=true;
    if (key == '=') deltaMove = 1.0;
	if (key == '-') deltaMove = -1.0;
	/*if (key == 's') {
        if(!pressedS){
            pressedS = true;
			randomBlock();
			nextblock = Block(col[choose]);

            createBlock(choose);
            glutPostRedisplay();
        }
	}
	*/
	if (key == '\/') {
		keep = 1;
	}
	if (key == 'z') {
        if(!start) start = true;
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
}
void DrawBorder(GLfloat *color) {
    glBegin(GL_LINES);
    glEnd();
        //x*2-5,-y*2+17,0
    /*glBegin(GL_QUADS);

    glColor3f(1,1,1);
    glVertex3f(-6,18,-1);
    glVertex3f(14,18,-1);
    glVertex3f(14,-22,-1);
    glVertex3f(-6,-22,-1);
    glEnd();
    */

    //block border
    for(int i=0; i<=21; i++) for(int j=0; j<=11; j++) {
        if(i==0||i==21||j==0||j==11) {
            glPushMatrix();
            Block b = Block(color);
            if(i==0) glTranslatef(j*2-7,19,0);
            else if(i==21) glTranslatef(j*2-7,-23,0);
            else if(j==0) glTranslatef(-7,-i*2+19,0);
            else if(j==11) glTranslatef(15,-i*2+19,0);
//                b.drawCube('t');
            glColor3f(b.color[0],b.color[1],b.color[2]);
            model.Draw();
            glPopMatrix();
        }
    }
}
void DrawGameBoard() {
    DrawBorder(white);
	DrawNext();
    int k = 0;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			if (board.board[i][j] || board.boardCurrent[i][j]) {
//				cout << k<< " " <<i << " " << j << endl;
				k++;
				glPushMatrix();
				Block b = board.blocks[i][j];
				glTranslatef(j*2-BOARD_X,-i*2+BOARD_Y,0);    //calculate location
//				b.drawCube('c');
                glColor3f(b.color[0],b.color[1],b.color[2]);
                model.Draw();
				glPopMatrix();
            }
            else if (board.boardShadow[i][j]) {
				glPushMatrix();
				Block b = Block(shadow);
				glTranslatef(j*2-BOARD_X,-i*2+BOARD_Y, 0);    //calculate location
				b.drawCube('c');
//				glColor3f(b.color[0], b.color[1], b.color[2]);
				model.Draw();
				glPopMatrix();
			}
		}
	}
}
void DrawHome() {
//    DrawBorder(red);
    for(int i=0; i<20; i++) {
        for(int j=0; j<10; j++) {
            glPushMatrix();
            glTranslatef(j*2-HOME_X,-i*2+HOME_Y,0);
            glColor3fv(pink);
            number[9].Draw();
    //        model.Draw();
            glPopMatrix();
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
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	setUplighting();
	glTranslatef(0+dx,0+dy,dz-68);
//	setUptexture("D:/_fang/year 3/cg/demotetris/texture/brick.bmp");
//	setUptexture("texture/brick.bmp");

    gluLookAt(
			x,      1,      z,               //camera location @ (x,1,z)
			x - lx, 1, z - lz,               // looking along vector (lx,1,lz)
			0,      1,      0);              // with up vector (0,1,0)

	DrawGameBoard();
	DrawHome();
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
    glutTimerFunc(50, updateframe, 0);
}
void update(int value) {
	if (!pause&&!board.currentColumn.empty()) {
        speed=(space)?10:1000;
		if (!board.movedown()) {
            space=false;
            createBlock(choose);
		}

		board.update();

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
	glutKeyboardUpFunc(releaseSpecialKey);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	model.Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/FONT/tetrisBlender/tetris.obj");
//	model.Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/cube-m.obj");
//	model.Load("model/cube-m.obj");
//	model.Load("model/cube.obj");

//    for(int i=0; i<10; i++) {
//        number[i].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/FONT/"+i+".obj");
//    }

    number[0].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/FONT/0.obj");
    number[1].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/FONT/1.obj");
    number[2].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/FONT/2.obj");
    number[3].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/FONT/3.obj");
    number[4].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/FONT/4.obj");
    number[5].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/FONT/5.obj");
    number[6].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/FONT/6.obj");
    number[7].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/FONT/7.obj");
    number[8].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/FONT/8.obj");
    number[9].Load("D:/_fang/year 3/cg/CGTetris/demotetris/model/FONT/9.obj");


	glutTimerFunc(speed, update, 0);
	glutTimerFunc(50, updateframe, 0);

	glutMainLoop();
	return 0;
}
