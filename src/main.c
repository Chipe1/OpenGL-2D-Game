#include "main.h"

Mat4 M, V, P, VP;
Mesh *tentacool_mesh, *ink_mesh, *default_mesh, *ground_mesh, *power_mesh, *hard_mesh, *weak_hard_mesh, *broken_hard_mesh, *soft_mesh, *weak_soft_mesh, *broken_soft_mesh, *arrow_mesh, *coin_mesh, *shine_coin_mesh, *fish_body_mesh, *fish_tail_mesh, *bubble_mesh;
Object *objects[MAX_OBJECTS], *staticobj[MAX_OBJECTS], *ground_obj, *power_obj, *arrow_obj, *fish_tail_obj, *bubble_obj;
int num_objects, score, free_fire, force_kill, num_staticobj, ready_to_fire, volley, special, show_arrow, num_anims;
Shader *current_shader, *default_shader;
float mouseX, mouseY, delta_t, cameraX, cameraY, zoomRatio, last_time, power, pow_vel, time_scale;
Anim *anims[MAX_OBJECTS];

int main(int argc, char **argv){
	
	GLFWwindow *window;
	Shader *shader;
	GLuint tex;
	int i, j;
	Vec3 v3;

	window=initGLFW();
	initGL();
	initGLEW();

	mouseX=0;mouseY=0;
	cameraX=0;cameraY=0;
	zoomRatio=1;
	num_objects=0;
	num_staticobj=0;
	score=0;
	free_fire=1;
	force_kill=0;
	ready_to_fire=0;
	pow_vel=5;
	volley=0;
	time_scale=1;
	show_arrow=0;
	num_anims=0;

	current_shader=NULL;
	default_shader=Shader_create("./res/shade.vs", "./res/shade.fs");
	
	tentacool_mesh=Mesh_create("./res/tentacool.mesh");
	default_mesh=Mesh_create("./res/square.mesh");
	ink_mesh=Mesh_create("./res/ink.mesh");
	ground_mesh=Mesh_create("./res/ground.mesh");
	power_mesh=Mesh_create("./res/power.mesh");
	hard_mesh=Mesh_create("./res/hard_obs.mesh");
	weak_hard_mesh=Mesh_create("./res/weak_hard_obs.mesh");
	broken_hard_mesh=Mesh_create("./res/broken_hard_obs.mesh");
	soft_mesh=Mesh_create("./res/soft_obs.mesh");
	weak_soft_mesh=Mesh_create("./res/weak_soft_obs.mesh");
	broken_soft_mesh=Mesh_create("./res/broken_soft_obs.mesh");
	arrow_mesh=Mesh_create("./res/arrow.mesh");
	coin_mesh=Mesh_create("./res/coin.mesh");
	shine_coin_mesh=Mesh_create("./res/shine_coin.mesh");
	fish_body_mesh=Mesh_create("./res/fish_body.mesh");
	fish_tail_mesh=Mesh_create("./res/fish_tail.mesh");
	bubble_mesh=Mesh_create("./res/bubble.mesh");

	tex=Texture_load("./res/rainbow.png");

	ground_obj=Object_create();
	Object_attachMesh(ground_obj, ground_mesh);
	ground_obj->pos.y=GROUND_LEVEL;

	arrow_obj=Object_create();
	Object_attachMesh(arrow_obj, arrow_mesh);
	
	power_obj=Object_create();
	Object_attachMesh(power_obj, power_mesh);
	power_obj->model.e[1][3]=1;
	power_obj->model.e[0][3]=-1;
	power_obj->model.e[0][0]=0.01;
	power_obj->model.e[1][1]=0.5;

	objects[num_objects++]=Object_create();
	objects[0]->flags|=TYPE_FIXED;
	objects[0]->life=99999;
	objects[0]->shader=default_shader;
	Object_attachMesh(objects[0], tentacool_mesh);
	objects[0]->pos=GLM_createVec3(-7, -3, 0);

	objects[num_objects++]=Object_create();
	objects[1]->flags|=TYPE_FIXED;
	objects[1]->life=1;
	objects[1]->points=1;
	objects[1]->radius=2;
	objects[1]->shader=default_shader;
	Object_attachMesh(objects[1], fish_body_mesh);
	objects[1]->pos=GLM_createVec3(-10, 9, 0);
	objects[1]->vel.x=3;

	fish_tail_obj=Object_create();
	fish_tail_obj->shader=default_shader;
	Object_attachMesh(fish_tail_obj, fish_tail_mesh);

	bubble_obj=Object_create();
	bubble_obj->shader=default_shader;
	Object_attachMesh(bubble_obj, bubble_mesh);
	bubble_obj->model.e[0][0]=bubble_obj->model.e[1][1]=0.15;
	bubble_obj->pos.z=0.2;

	loadLevel();

	P=GLM_orthoMat4(8, 4.5, -10, 10);
	V=GLM_viewMat4(GLM_createVec3(0, 0, 0), GLM_createVec3(0, 0, 1), GLM_createVec3(0, 1, 0));
	VP=GLM_mulMat4Mat4(P, V);
	
	/* game loop */
	last_time=glfwGetTime();
	while(!glfwWindowShouldClose(window)){
		if(num_objects>MAX_OBJECTS)fprintf(stderr, "Exceeded maximum of %d object. Please purchase the premium pack to unlock more objects\n", MAX_OBJECTS);
		delta_t=-last_time;
		last_time=glfwGetTime();
		delta_t+=last_time;
		delta_t*=time_scale;
		glfwPollEvents();
		manageInput();
		for(i=num_anims-1;i>=0;i--){
			if(anims[i]->time>anims[i]->length){
				free(anims[i]);
				for(j=i;j<num_anims-1;j++)
					anims[j]=anims[j+1];
				num_anims--;
			}
		}
		if(num_objects>1&&objects[1]->mesh==fish_body_mesh){
			objects[1]->pos.x+=objects[1]->vel.x*delta_t;
			if(objects[1]->pos.x>5){
				objects[1]->vel.x*=-1;
				objects[1]->model.e[0][0]*=-1;
				fish_tail_obj->model.e[0][0]*=-1;
			}
			else if(objects[1]->pos.x<-10){
				objects[1]->vel.x*=-1;
				objects[1]->model.e[0][0]*=-1;
				fish_tail_obj->model.e[0][0]*=-1;
			}
			fish_tail_obj->pos=objects[1]->pos;
			fish_tail_obj->pos.x-=objects[1]->vel.x/3;
			fish_tail_obj->theta=(sinf(last_time*1.3)*(PI/6))*fish_tail_obj->model.e[0][0];
		}
		if(show_arrow && !free_fire){
			for(i=0;i<num_objects;i++)
				if(objects[i]->flags&TYPE_WEAPON){
					arrow_obj->pos=objects[i]->pos;
					arrow_obj->theta=atan2f(mouseY-objects[i]->pos.y, mouseX-objects[i]->pos.x);
				}
		}
		if(ready_to_fire){
			power+=pow_vel*delta_t;
			if(power<2||power>12){
				pow_vel*=-1;
				power+=pow_vel*delta_t;
			}
			power_obj->model.e[0][0]=0.01+(0.328)*(power-2)/10;
		}
		if(force_kill){
			if(!free_fire){
				for(i=0;(!(objects[i]->flags&TYPE_WEAPON))&&i<num_objects;i++);
				Object_free(objects[i]);
				for(j=i;j<num_objects;j++){
					objects[j]=objects[j+1];
				}
				num_objects--;
				power_obj->model.e[0][0]=0.01;
				free_fire=1;
				ready_to_fire=0;
			}
			force_kill=0;
		}
		for(i=num_objects-1;i>=0;i--){
			if(objects[i]->life<=0){
				
				/* change mesh to broken */
				if(objects[i]->mesh==weak_hard_mesh)
					Object_attachMesh(objects[i], broken_hard_mesh);
				else if(objects[i]->mesh==weak_soft_mesh)
					Object_attachMesh(objects[i], broken_soft_mesh);
				else if(objects[i]->mesh==coin_mesh)
					Object_attachMesh(objects[i], shine_coin_mesh);

				if(objects[i]->death_time==0){
					objects[i]->death_time=last_time+1;	
				}
				else if(objects[i]->death_time<=last_time){
					num_objects--;
					if(objects[i]->flags&TYPE_WEAPON){
						free_fire=1;
						power_obj->model.e[0][0]=0.01;
					}
					score+=objects[i]->points;
					Object_free(objects[i]);
					for(j=i;j<num_objects;j++){
						objects[j]=objects[j+1];
					}
					continue;
				}
			}
			Object_update(objects[i]);
		}
		draw();
		glfwSwapBuffers(window);
	}

	printf("\n\n\nTHIS MUST NOT PRINT!!!\n");
	return 0;
}

void loadLevel(void){
	Object *to_add;
	int i;

	to_add=Object_create();
	Object_attachMesh(to_add, hard_mesh);
	to_add->radius=0.5;
	to_add->life=5;
	to_add->flags|=TYPE_FIXED;
	to_add->points=10;
	to_add->model.e[0][0]=to_add->radius;
	to_add->model.e[1][1]=to_add->radius;
	objects[num_objects++]=to_add;

	to_add=Object_create();
	Object_attachMesh(to_add, hard_mesh);
	to_add->radius=0.5;
	to_add->life=5;
	to_add->flags|=TYPE_FIXED;
	to_add->points=10;
	to_add->model.e[0][0]=to_add->radius;
	to_add->model.e[1][1]=to_add->radius;
	to_add->pos.x+=4;
	objects[num_objects++]=to_add;

	for(i=0;i<7;i++){
		to_add=Object_create();
		Object_attachMesh(to_add, hard_mesh);
		to_add->radius=0.5;
		to_add->life=5;
		to_add->flags|=TYPE_FIXED;
		to_add->points=10;
		to_add->model.e[0][0]=to_add->radius;
		to_add->model.e[1][1]=to_add->radius;
		to_add->pos.x+=5+i;
		to_add->pos.y+=2;
		objects[num_objects++]=to_add;

		to_add=Object_create();
		Object_attachMesh(to_add, hard_mesh);
		to_add->radius=0.5;
		to_add->life=5;
		to_add->flags|=TYPE_FIXED;
		to_add->points=10;
		to_add->model.e[0][0]=to_add->radius;
		to_add->model.e[1][1]=to_add->radius;
		to_add->pos.x+=5+i;
		to_add->pos.y+=5;
		objects[num_objects++]=to_add;
	}

	to_add=Object_create();
	Object_attachMesh(to_add, coin_mesh);
	to_add->radius=0.7;
	to_add->life=1;
	to_add->flags|=TYPE_LIGHT;
	to_add->points=to_add->radius*20;
	to_add->model.e[0][0]=to_add->radius;
	to_add->model.e[1][1]=to_add->radius;
	to_add->pos.x+=12;
	to_add->pos.y+=3.5;
	objects[num_objects++]=to_add;

	to_add=Object_create();
	Object_attachMesh(to_add, coin_mesh);
	to_add->radius=2;
	to_add->life=1;
	to_add->flags|=TYPE_FIXED;
	to_add->points=to_add->radius*20;
	to_add->model.e[0][0]=to_add->radius;
	to_add->model.e[1][1]=to_add->radius;
	to_add->pos.x=-7;
	to_add->pos.y+=20;
	objects[num_objects++]=to_add;

	to_add=Object_create();
	Object_attachMesh(to_add, coin_mesh);
	to_add->radius=0.4;
	to_add->life=1;
	to_add->flags|=TYPE_FIXED;
	to_add->points=to_add->radius*20;
	to_add->model.e[0][0]=to_add->radius;
	to_add->model.e[1][1]=to_add->radius;
	to_add->pos.x+=3;
	objects[num_objects++]=to_add;

	to_add=Object_create();
	Object_attachMesh(to_add, coin_mesh);
	to_add->radius=0.6;
	to_add->life=1;
	to_add->points=to_add->radius*20;
	to_add->model.e[0][0]=to_add->radius;
	to_add->model.e[1][1]=to_add->radius;
	to_add->pos=GLM_createVec3(1.5, GROUND_LEVEL+to_add->radius, 0);
	objects[num_objects++]=to_add;

	for(i=0;i<3;i++){
		to_add=Object_create();
		Object_attachMesh(to_add, coin_mesh);
		to_add->radius=1;
		to_add->life=1;
		to_add->flags|=TYPE_FIXED;
		to_add->points=to_add->radius*20;
		to_add->model.e[0][0]=to_add->radius;
		to_add->model.e[1][1]=to_add->radius;
		to_add->pos=GLM_createVec3(-10, 2+2*i, 0);
		objects[num_objects++]=to_add;
	}

	to_add=Object_create();
	Object_attachMesh(to_add, soft_mesh);
	to_add->radius=0.4;
	to_add->life=2;
	to_add->mass=to_add->radius*5;
	to_add->model.e[0][0]=to_add->radius;
	to_add->model.e[1][1]=to_add->radius;
	to_add->pos=GLM_createVec3(0, GROUND_LEVEL+to_add->radius, 0);
	objects[num_objects++]=to_add;

	to_add=Object_create();
	Object_attachMesh(to_add, soft_mesh);
	to_add->radius=0.8;
	to_add->life=2;
	to_add->mass=to_add->radius*5;
	to_add->model.e[0][0]=to_add->radius;
	to_add->model.e[1][1]=to_add->radius;
	to_add->pos=GLM_createVec3(3, GROUND_LEVEL+to_add->radius, 0);
	objects[num_objects++]=to_add;

}

void draw(void){

	int i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Object_render(ground_obj);
	for(i=0;i<num_objects;i++){
		Object_render(objects[i]);		
	}
	if(show_arrow && !free_fire)
		Object_render(arrow_obj);
	if(num_objects>1&&objects[1]->mesh==fish_body_mesh)
		Object_render(fish_tail_obj);
	for(i=0;i<num_anims;i++)
		Anim_render(anims[i]);
	Object_draw(power_obj);
}

void manageInput(void){
	objects[0]->theta=atan2f(mouseX-objects[0]->pos.x, objects[0]->pos.y-mouseY);
}

GLFWwindow* initGLFW (void){
	GLFWwindow* window; // window desciptor/handle
	
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_SAMPLES, AA_LEVEL);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	window = glfwCreateWindow(WIDTH, HEIGHT, GAME_NAME, NULL, NULL);
	
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	glfwSetWindowCloseCallback(window, quit);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_press_callback);
	glfwSetKeyCallback(window, key_callback);

	return window;
}

void initGL(void){
        // Background color of the scene
        glClearColor(0.3f, 0.3f, 0.7f, 0.0f); // R, G, B, A
        glClearDepth(1.0f);
	
	/* Enable occulision culling  */
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
	glDepthRange(1,-1);
	
	/* print OpenGL info  */
        printf("VENDOR:\t%s\n",glGetString(GL_VENDOR));
        printf("RENDERER:\t%s\n",glGetString(GL_RENDERER));
        printf("VERSION:\t%s\n",glGetString(GL_VERSION));
        printf("GLSL:\t%s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void initGLEW(void){
        glewExperimental = GL_TRUE;
        if(glewInit()!=GLEW_OK){
                fprintf(stderr,"Glew failed to initialize : %s\n", glewGetErrorString(glewInit()));
        }
        if(!GLEW_VERSION_3_3)
		fprintf(stderr, "3.3 version not available\n");
}

void error_callback(int error, const char* description){
	fprintf(stderr, "Error No. %d : %s\n", error, description);
}

void quit(GLFWwindow *window){
	glfwDestroyWindow(window);
	glfwTerminate();
	printf("Score : %d\n", score);
	exit(EXIT_SUCCESS);
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos){
	float normX, normY;
	normX=(float)((16.0/WIDTH)*(xpos-WIDTH/2.0));
	normY=-(float)((9.0/HEIGHT)*(ypos-HEIGHT/2.0));
	normX/=zoomRatio;normY/=zoomRatio;
	//	normX+=cameraX;normY+=cameraY;

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS){
		cameraX=mouseX-normX;
		cameraY=mouseY-normY;
		V=GLM_viewMat4(GLM_createVec3(cameraX, cameraY, 0), GLM_createVec3(0, 0, 1), GLM_createVec3(0, 1, 0));
		VP=GLM_mulMat4Mat4(P, V);
	}
	else{
		mouseX=normX+cameraX;
		mouseY=normY+cameraY;
	}
}

void mouse_scroll_callback(GLFWwindow *window, double xoff, double yoff){
	if(yoff>0)
		zoomRatio*=1.05;
	else if(yoff<0)
		zoomRatio/=1.05;
	else
		return ;

	//Zoom in and out by changing the projection matrix
	P=GLM_orthoMat4(8/zoomRatio, 4.5/zoomRatio, -10, 10);

	//Make the scroll change the mouse center to zoom in on focused point
	/* cameraX=mouseX; */
	/* cameraY=mouseY; */
	/* V=GLM_viewMat4(GLM_createVec3(cameraX, cameraY, 0), GLM_createVec3(0, 0, 1), GLM_createVec3(0, 1, 0)); */

	VP=GLM_mulMat4Mat4(P, V);
}

void mouse_press_callback(GLFWwindow *window, int button, int action, int mods){
	Object *ink_obj;
	if(button==GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS&&free_fire&&!ready_to_fire){
		ready_to_fire=1;
		power=2;
	}
	if(button==GLFW_MOUSE_BUTTON_LEFT && action==GLFW_RELEASE&&free_fire){
		ink_obj=Object_create();
		Object_attachMesh(ink_obj, ink_mesh);
		ink_obj->radius=0.2;
		ink_obj->model.e[0][0]=ink_obj->radius;
		ink_obj->model.e[1][1]=ink_obj->radius;
		ink_obj->pos=objects[0]->pos;
		ink_obj->pos.x+=(ink_obj->radius+objects[0]->radius)*cosf(objects[0]->theta-PI/2);
		ink_obj->pos.y+=(ink_obj->radius+objects[0]->radius)*sinf(objects[0]->theta-PI/2);
		ink_obj->vel=GLM_createVec3(power*cosf(objects[0]->theta-PI/2), power*sinf(objects[0]->theta-PI/2), 0);
		ink_obj->flags|=TYPE_WEAPON;
		ink_obj->points=0;
		volley++;
		
		objects[num_objects++]=ink_obj;
		ready_to_fire=0;
		special=1;
		free_fire=0;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	int i;

	/* force killing */
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
		force_kill=1;

	/* ink power */
	if(key==GLFW_KEY_B && action==GLFW_PRESS && special){
		show_arrow=1;
	}
	else if(key==GLFW_KEY_B && action==GLFW_RELEASE){
		Vec3 boost;
		show_arrow=0;
		if(special){
			for(i=0;i<num_objects;i++)
				if(objects[i]->flags&TYPE_WEAPON){
					boost=GLM_subVec3(GLM_createVec3(mouseX, mouseY, 0), objects[i]->pos);
					if(GLM_magVec3(boost)==0)
						boost=GLM_createVec3(1, 0, 0);
					else
						boost=GLM_unitVec3(boost);
					objects[i]->vel=GLM_mulScalarVec3(10, boost);
				}
			special=0;
		}
	}
	else if(key==GLFW_KEY_T && action==GLFW_PRESS && special){
		show_arrow=1;
	}
	else if(key==GLFW_KEY_T && action==GLFW_RELEASE){
		Vec3 teleport;
		show_arrow=0;
		if(special){
			for(i=0;i<num_objects;i++)
				if(objects[i]->flags&TYPE_WEAPON){
					teleport=GLM_subVec3(GLM_createVec3(mouseX, mouseY, 0), objects[i]->pos);
					if(GLM_magVec3(teleport)==0)
						teleport=GLM_createVec3(1, 0, 0);
					else
						teleport=GLM_unitVec3(teleport);
					objects[i]->pos=GLM_addVec3(objects[i]->pos, GLM_mulScalarVec3(2, teleport));
					objects[i]->vel=GLM_subVec3(objects[i]->vel, GLM_projectVec3(objects[i]->vel, teleport));
					objects[i]->vel=GLM_addVec3(objects[i]->vel, teleport);
				}
			special=0;
		}
	}

	/* time control */
	if(key==GLFW_KEY_Z && action==GLFW_PRESS){
		time_scale*=0.5;
		time_scale=MAX(time_scale, 0.125);
	}
	else if(key==GLFW_KEY_X && action==GLFW_PRESS){
		time_scale*=2;
		time_scale=MIN(time_scale, 4);
	}

}

