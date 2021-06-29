#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_Texture;
void main()
{
	Color_ = vec4(texture(u_Texture, v2f_TexCoords).rgb, 1.0f);
}