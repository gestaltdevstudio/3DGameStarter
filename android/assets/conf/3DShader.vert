#version 330 core

// Per-vertex
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexUV;

// Per-instance
layout(location = 2) in vec4 i_ModelRow0;
layout(location = 3) in vec4 i_ModelRow1;
layout(location = 4) in vec4 i_ModelRow2;
layout(location = 5) in vec4 i_ModelRow3;
layout(location = 6) in vec4 i_Color;
layout(location = 7) in int  i_TexIndex;

uniform mat4 u_ViewProj;

out vec2 v_UV;
out vec4 v_Color;
flat out int v_TexIndex;

void main()
{
    mat4 model  = mat4(i_ModelRow0, i_ModelRow1, i_ModelRow2, i_ModelRow3);
    gl_Position = u_ViewProj * model * vec4(vertexPos, 1.0);
    v_UV        = vertexUV;
    v_Color     = i_Color;
    v_TexIndex  = i_TexIndex;
}

