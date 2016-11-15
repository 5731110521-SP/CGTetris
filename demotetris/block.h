#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include <GL/glut.h>
#include "vec3f.h"
#include "modelloader.h"
class Block{
    public :
        char type;
        Vec3f pos;
        Vec3f angle;
        GLfloat *color;
        Model_OBJ model;

    public:
        Block();
        Block(char type,GLfloat *color,Vec3f pos,Vec3f angle,Model_OBJ model);
        ~Block();

        void addPos(Vec3f pos);
        void addAngle(char axis,float angle);

        void drawRect(char type,float a,float rx,float ry,float rz);
        void drawCube(char type,float tx,float ty,float tz);
        void drawBlockTexture(Block b);
        void drawBlockModel(Block b);
};

#endif
