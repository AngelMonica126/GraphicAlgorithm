#version 430 core

in  vec2 v2f_TexCoords;
layout (location = 0) out float FragColor_;

uniform sampler2D u_HBAOTexture;


void main()
{
    vec2 TexelSize = 1.0 / vec2(textureSize(u_HBAOTexture, 0));
    float Result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 Offset = vec2(float(x), float(y)) * TexelSize;
            Result += texture(u_HBAOTexture, v2f_TexCoords + Offset).r;
        }
    }
    FragColor_ = Result / (4.0 * 4.0);
}