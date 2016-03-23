#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 uv_coords;
layout(location=2)  in vec3 norm;

uniform mat4 MVP;

out vec2 uv;
out vec3 normal;

void main(){
	uv=uv_coords;
	normal=norm;
	gl_Position=MVP*vec4(position,1);
}
