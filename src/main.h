#define STB_IMAGE_IMPLEMENTATION
#include "struct.h"
#include "shader.h"
#include "glm.h"
#include "mesh.h"
#include "object.h"
#include "texture.h"
#include "anim.h"

void loadLevel(void);
void draw(void);
void manageInput(void);
GLFWwindow* initGLFW (void);
void initGL(void);
void initGLEW(void);
void error_callback(int, const char*);
void quit(GLFWwindow *);
void cursor_position_callback(GLFWwindow *, double, double);
void mouse_scroll_callback(GLFWwindow *, double, double);
void mouse_press_callback(GLFWwindow *, int, int, int);
void key_callback(GLFWwindow *, int, int, int, int);
