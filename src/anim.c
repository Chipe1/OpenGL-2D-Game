#include "anim.h"

extern float delta_t;
extern Object *bubble_obj;
extern void Object_render(Object *);

Anim *Anim_create(Vec3 pos, float length){
	Anim *toret=(Anim *)malloc(sizeof(Anim));
	toret->start=pos;
	toret->length=length;
	toret->time=0;
	return toret;
}

void Anim_render(Anim *anim){
	int i;
	float prev_size;

	prev_size=bubble_obj->model.e[0][0];
	anim->time+=delta_t;
	for(i=0;i<7;i++){
		bubble_obj->pos.x=anim->start.x-0.5+i/6.0+0.3*sinf((float)(i+anim->time*2)*PI/3.0);
		bubble_obj->pos.y=anim->start.y+sin(i)*0.3+anim->time;
		bubble_obj->model.e[0][0]=bubble_obj->model.e[1][1]=(0.5+i/8.0)*prev_size;
		Object_render(bubble_obj);
	}
		bubble_obj->model.e[0][0]=bubble_obj->model.e[1][1]=prev_size;
}
