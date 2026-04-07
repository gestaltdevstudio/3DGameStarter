#version 330 core

// =======================
// Vertex base do quad
// =======================
layout (location = 0) in vec2 a_Position; // (-0.5 .. 0.5)
//layout (location = 1) in vec2 a_TexCoord; // (0 .. 1)

// =======================
// Instance data
// =======================
layout (location = 1) in vec2 i_Position;
layout (location = 2) in vec2 i_Size;
layout (location = 3) in float i_Rotation;
layout (location = 4) in vec4 i_UV;        // x, y, w, h
layout (location = 5) in vec4 i_Color;
layout (location = 6) in float i_Z;
layout (location = 7) in float i_Alpha;
layout (location = 8) in int i_TexIndex;

flat out int v_TexIndex;

// =======================
// Uniforms
// =======================
uniform mat4 u_ViewProj;

// =======================
// Outputs
// =======================
out vec2 v_TexCoord;
out vec4 v_Color;

void main()
{
    // escala
    vec2 pos = a_Position * i_Size;
    
    v_TexIndex = i_TexIndex;

    // rotação 2D
    float c = cos(i_Rotation);
    float s = sin(i_Rotation);
    pos = vec2(
        pos.x * c - pos.y * s,
        pos.x * s + pos.y * c
    );

    // posição final
    vec3 worldPos = vec3(pos + i_Position, i_Z);
    
//    vec2 worldPos = a_Position;
    gl_Position = u_ViewProj * vec4(worldPos, 1.0);
//    gl_Position = vec4(a_Position / 100.0, 0.0, 1.0);


    // UV vindo do atlas
    vec2 baseUV = a_Position + 0.5;

    // NÃO mexe no atlas
    v_TexCoord = vec2(baseUV.x, 1.0 - baseUV.y) * i_UV.zw + i_UV.xy;
    
//    v_TexCoord = (a_Position + 0.5) * i_UV.zw + i_UV.xy;


    // cor / alpha
    v_Color = vec4(i_Color.rgb, i_Color.a * i_Alpha);    
}

