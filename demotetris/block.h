#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED
#include "vec3f.h"
#include "modelloader.h"
class Block{
    private :
        char type;
        Vec3f pos;
        Vec3f angle;
        Model_OBJ model;
		float wall_left;
		float wall_right;
		float wall_top;
		float wall_bottom;
		float wall_front;
		float wall_back;

    public:
        Block();
        Block(char type,Vec3f pos,Vec3f angle,Model_OBJ model);
        ~Block();

        void addPos(Vec3f pos);
        void addAngle(char axis,float angle);

        char getType();
        float getPos(char axis);
        Vec3f getAngle();
        float getAngle(char axis);
        Model_OBJ getModel();
		float getTopWall();
		float getBottomWall();
		float getLeftWall();
		float getRightWall();
		float getFrontWall();
		float getBackWall();

        void setType(char type);
        void setPos(Vec3f pos);
        void setAngle(Vec3f angle);
        void setAngle(char axis,float angle);
        void setModel(Model_OBJ model);
		void setTopWall(float t);
		void setBottomWall(float b);
		void setLeftWall(float l);
		void setRightWall(float r);
		void setFrontWall(float f);
		void setBackWall(float b);
};

#endif