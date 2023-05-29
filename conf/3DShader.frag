#version 330 core

// Ouput data
out vec4 color;

void main(){

	// Output color = color specified in the vertex shader, 
	// interpolated between all 3 surrounding vertices
	color = vec4(1.0, 1.0, 1.0, 0.2);

}
