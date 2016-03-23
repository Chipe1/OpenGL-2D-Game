#ifndef __openglstruct__
#define __openglstruct__
#include "include.h"

enum{
        POSITION_VB,
        UV_VB,
	NORMAL_VB,
        INDEX_VB,
        NUM_BUFFERS
};

enum{
        TRANSFORM_U,
        NUM_UNIFORMS
};

enum{
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        NUM_SHADERS
};

enum{
	TYPE_FIXED=1,
	TYPE_WALL=1<<1,
	TYPE_IMMORTAL=1<<2,
	TYPE_LIGHT=1<<3,
	TYPE_WEAPON=1<<4,
	
};

typedef struct __ogl_Shader{
        GLuint prog,shaders[NUM_SHADERS],uniforms[NUM_UNIFORMS];
}Shader;

typedef struct __ogl_Mesh{
        GLuint buffers[NUM_BUFFERS];
        int drawCount;
}Mesh;

typedef struct __ogl_Vec3{
        float x,y,z;
}Vec3;

typedef struct __ogl_Vec4{
        float x,y,z,w;
}Vec4;

typedef struct __ogl_Mat4{
        float e[4][4];
}Mat4;

typedef struct __ogl_Object{
	GLuint vao;
	Mesh *mesh;
	Shader *shader;
	Mat4 model;
	Vec3 pos;
	Vec3 vel;
	Vec3 acc;
	float mass;
	float theta;
	float omega;
	float radius;
	int flags;
	int life;
	int points;
	int hit_volley;
	float death_time;
}Object;

typedef struct __ogl_Anim{
	Vec3 start;
	float length;
	float time;
}Anim;
#endif
