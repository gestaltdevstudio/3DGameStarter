#version 300 es

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPos;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){	

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPos,1);
	
}

