#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<GL/glew.h>
#include<GL/gl.h>
#include<GLFW/glfw3.h>
#include "stb_image.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define PI 3.14159265358979323846
#define WIDTH 1600
#define HEIGHT 900
#define FPS 60
#define AA_LEVEL 1
#define GAME_NAME "My game"
#define MAX_OBJECTS 100
#define GRAVITY 3.5
#define AIR_FRICTION 0.3
#define FRICTION 0.1
#define GROUND_LEVEL -4.3
#define ELASTICITY 0.7
#define EPS 0.1
