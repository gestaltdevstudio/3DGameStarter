#version 330 core

uniform sampler2D u_Textures[8];
uniform int       u_UseTexture;

in  vec2 v_UV;
in  vec4 v_Color;
flat in int v_TexIndex;

out vec4 color;

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
        default: return vec4(1.0, 0.0, 1.0, 1.0); // magenta = debug
    }
}

void main()
{
    if (u_UseTexture == 1)
    {
        vec4 texColor = sampleTexture(v_TexIndex, v_UV);
        color = vec4(texColor.rgb * v_Color.rgb, texColor.a * v_Color.a);
    }
    else
    {
        color = v_Color;
    }

    if (color.a < 0.01)
        discard;
}

