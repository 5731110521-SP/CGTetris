#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <set>
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

using namespace std;

#define TO_RAD 3.1415926535897932384626433832795/180.0
#define BLOCK_NUMBER 1000
#define WIDTH 10
#define HEIGHT 10
#define DEPTH 20

Board board;
int choose = -1;
int current=0;
int random = 0;
vector<int> typeBlock = { 0,1,1,2,3,3,4,4,5,5,6,7 };
//int typeBlock[] = {0,1,1,2,3,3,4,4,5,5,6,7};
int box[WIDTH][HEIGHT][DEPTH];
vector<Block> _blocks;
Model_OBJ t_model,l_model;
int _axis;
float _angle[3];
GLfloat mat[16];
GLuint _textureId;
GLfloat red[] = { 1,0,0 };
GLfloat green[] = { 0,1,0 };
GLfloat blue[] = { 0,0,1 };

void createBlock(int random,Block block) {
	if (random == 0) { // ---
		board.addblocks(block, 1, 3);
		board.addblocks(block, 1, 4);
		board.addblocks(block, 1, 5);
		board.addblocks(block, 1, 6);
		board.currentPointRow = 0;
		board.currentPointColumn = 3;
	}
	else if (random == 1) {				// - 
		board.addblocks(block, 2, 1);	// ---
		board.addblocks(block, 2, 2);
		board.addblocks(block, 2, 3);
		board.addblocks(block, 1, 1);
		board.currentPointRow = 0;
		board.currentPointColumn = 0;
	}
	else if (random == 2) {				// --
		board.addblocks(block, 1, 1);	// --
		board.addblocks(block, 1, 2);
		board.addblocks(block, 2, 1);
		board.addblocks(block, 2, 2);
		board.currentPointRow = 0;
		board.currentPointColumn = 0;
	}
	else if (random == 3) {				//  -
		board.addblocks(block, 1, 1);	// ---
		board.addblocks(block, 1, 2);
		board.addblocks(block, 1, 3);
		board.addblocks(block, 2, 3);
		board.currentPointRow = 0;
		board.currentPointColumn = 0;
	}
	else if (random == 4) {				//  --
		board.addblocks(block, 2, 2);	//--
		board.addblocks(block, 2, 3);
		board.addblocks(block, 1, 1);
		board.addblocks(block, 1, 2);
		board.currentPointRow = 0;
		board.currentPointColumn = 0;
	}
	else if (random == 5) {				//--
		board.addblocks(block, 2, 1);	//  --
		board.addblocks(block, 2, 2);
		board.addblocks(block, 1, 2);
		board.addblocks(block, 1, 3);
		board.currentPointRow = 0;
		board.currentPointColumn = 0;
	}
	else if(random == 6){
		board.addblocks(block, 2, 2);	//  -
		board.addblocks(block, 2, 3);	//---
		board.addblocks(block, 2, 4);
		board.addblocks(block, 1, 4);
		board.currentPointRow = 0;
		board.currentPointColumn = 0;
	}
	else {
		board.addblocks(block, 1, 1);	//  -
		board.addblocks(block, 2, 1);	//  -
		board.addblocks(block, 3, 1);	//  -
		board.addblocks(block, 4, 1);	//  -
		board.currentPointRow = 1;
		board.currentPointColumn = 0;
	}
//	board.addblocks(block, 0, 4);
//	board.addblocks(block, 0, 5);
//	board.addblocks(block, 1, 5);
//	board.addblocks(block, 1, 6);
	glutPostRedisplay();
}

//Returns a random float from 0 to < 1
float randomFloat() {
    srand(time(NULL));
	return (float)rand() / ((float)RAND_MAX + 1);
}
//Deletes everything.  This should be called when exiting the program.
void cleanup() {
//	for(unsigned int i = 0; i < _blocks.size(); i++) {
		delete[] &_blocks;
//	}
}
void handleArrow(int key,int x,int y) {
<<<<<<< HEAD
    if (key == GLUT_KEY_LEFT) updateT(-2, 0, 0);
	if (key == GLUT_KEY_DOWN) updateT(0, -2, 0);
	if (key == GLUT_KEY_RIGHT) updateT(2, 0, 0);
	if (key == GLUT_KEY_UP) updateT(0, 2, 0);
}

void handleKeypress(unsigned char key,int x,int y) {
    if (key == 27) { cleanup(); exit(0); }
    if (key == 'z') updateR(90, 'x');
    if (key == 'x') updateR(90, 'y');
    if (key == 'c') updateR(90, 'z');
    if (key == '.') updateT(0,0,-2);
    if (key == '\/') updateT(0,0,2);
    if (key == 'n') {
        current++;
        Vec3f pos = Vec3f(8 * randomFloat() - 4,
                          5,
                          8 * randomFloat() - 4);
        Block block = Block('t',pos,Vec3f(0,0,0),t_model);
        _blocks.push_back(block);
    }
	if (key == 's') {
		Block block = Block('t', Vec3f(0, 9, 8), Vec3f(0, 0, 0), t_model);

		random_shuffle(typeBlock.begin(),typeBlock.end());
		random = rand() % 12;
		createBlock(typeBlock[random],block);
		glutPostRedisplay();
	}
	if (key == '1') {
		if (!board.currentColumn.empty()) {
			board.moveblock(-1);
			glutPostRedisplay();
		}
	}
	if (key == '2') {
		if (!board.currentColumn.empty()) {
			board.moveblock(1);
			glutPostRedisplay();
		}
	}
	if (key == '3') {
		if (!board.currentColumn.empty()) {
			if (!board.movedown()) {
				Block block = Block('t', Vec3f(0, 9, 8), Vec3f(0, 0, 0), t_model);

				random_shuffle(typeBlock.begin(), typeBlock.end());
				random = rand() % 12;
				if (typeBlock[random] != choose) choose = typeBlock[random];
				else choose = typeBlock[random/2];
				createBlock(choose, block);
=======
    if (key == GLUT_KEY_LEFT) {
        if (!board.currentColumn.empty()) {
            board.moveblock(-1);
            glutPostRedisplay();
		}
    }
	if (key == GLUT_KEY_DOWN) {
        if (!board.currentColumn.empty()) {
			if (!board.movedown()) {                //create new wave
				Block block = Block('t', blue,Vec3f(0, 9, 8), Vec3f(0, 0, 0), t_model);
				board.addblocks(block, 0, 4);       //z shape
				board.addblocks(block, 0, 5);
				board.addblocks(block, 1, 5);
				board.addblocks(block, 1, 6);
>>>>>>> refs/remotes/origin/GIZ
			}

			board.update();
			glutPostRedisplay();
		}
	}
<<<<<<< HEAD
	if (key == '4') {
		if (!board.currentColumn.empty()) {
			board.rotateBlock();
			glutPostRedisplay();
		}
	}
=======
	if (key == GLUT_KEY_RIGHT) {
        if (!board.currentColumn.empty()) {
			board.moveblock(1);
			glutPostRedisplay();
		}
	}
	if (key == GLUT_KEY_UP) {  }
}
void handleKeypress(unsigned char key,int x,int y) {
    if (key == 27) { cleanup(); exit(0); }
	if (key == 's') {                           //create new block
		Block block = Block('t', blue,Vec3f(0, 9, 8), Vec3f(0, 0, 0), t_model);
		board.addblocks(block, 0, 4);           //z shape
		board.addblocks(block, 0, 5);
		board.addblocks(block, 1, 5);
		board.addblocks(block, 1, 6);
		glutPostRedisplay();
	}
>>>>>>> refs/remotes/origin/GIZ
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
	Vec3f pos = Vec3f(8 * randomFloat() - 4,
                          5,
                          8 * randomFloat() - 4);
//    Block block = Block('t',pos,Vec3f(0,0,0),t_model);
//	//Block block = Block('t', Vec3f(0,9,8), Vec3f(0, 0, 0), t_model);
//    _blocks.push_back(block);
}
void DrawBorder() {
    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(-6,-18,0);
        glVertex3f(-6,18,0);

        glVertex3f(6,-18,0);
        glVertex3f(6,18,0);

        glVertex3f(-6,-18,0);
        glVertex3f(6,-18,0);

        glVertex3f(-6,18,0);
        glVertex3f(6,18,0);

    glEnd();
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
	glTranslatef(0,0,-70);
//	setUptexture("D:/_fang/year 3/cg/demotetris/texture/watertexture.bmp");
//	setUptexture("D:/_fang/year 3/cg/demotetris/texture/crate.bmp");
//	setUptexture("D:/_fang/year 3/cg/demotetris/texture/brick.bmp");
//	setUptexture("texture/brick.bmp");

	cout<<"current "<<current<<endl;
	cout<<"num block "<<_blocks.size()<<endl;

	/*      model giz
	for(int i=0; i<_blocks.size(); i++) {
        cout<<"push"<<endl;
        glPushMatrix();
        glTranslatef(_blocks[i].getPos('x'),
                     _blocks[i].getPos('y'),
                     _blocks[i].getPos('z'));
        cout<<"translated"<<endl;
        glRotatef(_blocks[i].getAngle('x'),1,0,0);
        glRotatef(_blocks[i].getAngle('y'),0,1,0);
        glRotatef(_blocks[i].getAngle('z'),0,0,1);
        cout<<"rotated"<<endl;
        if(_blocks[i].getType()=='t') _blocks[i].setModel(t_model);
        if(_blocks[i].getType()=='l') _blocks[i].setModel(l_model);
        cout<<"model set"<<endl;
        _blocks[i].getModel().Draw();
    //        if(b.getModel()==t_model) cout<<"correct model"<<endl;
        cout<<"pop"<<endl;
        glPopMatrix();
	}
    */

	/*      texture/color giz
	for(int i=0; i<_blocks.size(); i++) {
        glPushMatrix();
        glTranslatef(_blocks[i].getPos('x'),
                     _blocks[i].getPos('y'),
                     _blocks[i].getPos('z'));
        glRotatef(_blocks[i].getAngle('x'),1,0,0);
        glRotatef(_blocks[i].getAngle('y'),0,1,0);
        glRotatef(_blocks[i].getAngle('z'),0,0,1);
        drawCube('c',*blue,0, 0, 0);
        drawCube('c',*red,2, 0, 0);
        drawCube('c',*blue,2, 2, 0);
        glPopMatrix();
	}
	*/

//	DrawBorder();
	int k = 0;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
            glBegin(GL_QUADS);
            glColor3f(1,1,1);
            glVertex3f(-6,18,0);
            glVertex3f(14,18,0);
            glVertex3f(14,-22,0);
            glVertex3f(-6,-22,0);
            glEnd();
			if (board.board[i][j] || board.boardCurrent[i][j]) {
				cout << k<< " " <<i << " " << j << endl;
				k++;
				glPushMatrix();
				Block b = board.blocks[i][j];
				b.color=blue;
				glTranslatef(j*2-5,-i*2+17,0);    //set initial location
				glRotatef(0, 1, 0, 0);
				glRotatef(0, 0, 1, 0);
				glRotatef(0, 0, 0, 1);
				b.drawCube('c', 0, 0, 0);
				glPopMatrix();
			}
		}
	}

	glutSwapBuffers();
}

void update(int value) {
	if (!board.currentColumn.empty()) {
		if (!board.movedown()) {
<<<<<<< HEAD
			Block block = Block('t', Vec3f(0, 9, 8), Vec3f(0, 0, 0), t_model);

			random = rand() % 5;
			createBlock(random, block);

=======
			Block block = Block('t', blue,Vec3f(0, 9, 8), Vec3f(0, 0, 0), t_model);
			board.addblocks(block, 0, 4);
			board.addblocks(block, 0, 5);
			board.addblocks(block, 1, 5);
			board.addblocks(block, 1, 6);
>>>>>>> refs/remotes/origin/GIZ
		}

		board.update();

		glutPostRedisplay();
	}
	glutTimerFunc(1000, update, 0);
}

//Called every TIMER_MS milliseconds
//void update(int value) {
//	advance(_blocks, _octree, (float)TIMER_MS / 1000.0f, _timeUntilUpdate);
//	glutPostRedisplay();
//	glutTimerFunc(TIMER_MS, update, 0);
//}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("TETRIS ><");
	initRendering();

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleArrow);
	glutReshapeFunc(handleResize);
	t_model.Load("D:/_fang/year 3/cg/demotetris/model/t-tetris-m.obj");
	l_model.Load("D:/_fang/year 3/cg/demotetris/model/l-tetris-m.obj");
//	t_model.Load("model/t-tetris-m.obj");
//	l_model.Load("model/l-tetris-m.obj");

	glutTimerFunc(1000, update, 0);

	glutMainLoop();
	return 0;
}
