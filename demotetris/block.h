#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include <GL/glut.h>
#include "vec3f.h"
#include "modelloader.h"
class Block{
    public :
        GLfloat *color;
        Model_OBJ model;

    public:
        Block();
        Block(GLfloat *color);
        ~Block();

        void drawRect(char type,float a,float rx,float ry,float rz);
        void drawCube(char type);
};

#endif
