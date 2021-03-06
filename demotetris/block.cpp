#include <assert.h>
#include <fstream>

#include <GL/glut.h>
#include "block.h"
#include "vec3f.h"
#include "modelloader.h"

using namespace std;

Block::Block() {}
Block::Block(GLfloat *c) : color(c) {}

void Block::drawRect(char type, float a, float rx, float ry, float rz) {
	glPushMatrix();
	glRotatef(a, rx, ry, rz);
	glBegin(GL_QUADS);
	if (type == 't') glTexCoord2f(0, 0);
	glVertex3f(-1, -1, 1);
	if (type == 't') glTexCoord2f(1, 0);
	glVertex3f(1, -1, 1);
	if (type == 't') glTexCoord2f(1, 1);
	glVertex3f(1, 1, 1);
	if (type == 't') glTexCoord2f(0, 1);
	glVertex3f(-1, 1, 1);
	glEnd();
	glPopMatrix();
}
void Block::drawCube(char type) {
	glPushMatrix();
	if (type == 'c') {
		glColor3f(color[0], color[1], color[2]);
	}
	if (type == 't') {
		glEnable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);
	}
	drawRect(type, 0, 0, 0, 0);
	drawRect(type, 90, 1, 0, 0);
	drawRect(type, -90, 1, 0, 0);
	drawRect(type, 180, 1, 0, 0);
	drawRect(type, 90, 0, 1, 0);
	drawRect(type, -90, 0, 1, 0);
	glPopMatrix();
}
