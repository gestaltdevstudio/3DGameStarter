#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
uniform mat4 MVP;

void main()
{
	//vec2 vertexPosition_homoneneousspace = vec2(aPos.x, aPos.y) - vec2(400,300); // [0..800][0..600] -> [-400..400][-300..300]
	//vertexPosition_homoneneousspace /= vec2(400,300);
	//gl_Position =  vec4(aPos.x, aPos.y,0,1);
	gl_Position = MVP * vec4(aPos.x, aPos.y, 0, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
