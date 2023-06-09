#version 300 es
layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 vertexUV;

out vec2 UV;
uniform mat4 MVP;

void main(){

	gl_Position =  MVP * vec4(vertexPosition,0,1);	
	UV = vertexUV;

}
