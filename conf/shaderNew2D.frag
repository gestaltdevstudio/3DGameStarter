#version 330 core

in vec2 v_TexCoord;
in vec4 v_Color;
flat in int v_TexIndex;

out vec4 FragColor;

uniform sampler2D u_Textures[8];


vec4 sampleTexture(int index, vec2 uv)
{
    switch (index)
    {
        case 0: return texture(u_Textures[0], uv);
        case 1: return texture(u_Textures[1], uv);
        case 2: return texture(u_Textures[2], uv);
        case 3: return texture(u_Textures[3], uv);
        case 4: return texture(u_Textures[4], uv);
        case 5: return texture(u_Textures[5], uv);
        case 6: return texture(u_Textures[6], uv);
        case 7: return texture(u_Textures[7], uv);
        default: return vec4(1, 0, 1, 1);
    }
}

void main()
{
    vec4 texColor = sampleTexture(v_TexIndex, v_TexCoord);

    // Blend de cor (ex: piscar vermelho)
    FragColor = texColor * v_Color;
//    FragColor = vec4(1,0,0,1);
//    FragColor = v_Color;



    // Corte simples de alpha (opcional)
    if (FragColor.a < 0.01)
        discard;
}

