#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_Texture2D;

void main()
{
	Color_ = texture(u_Texture2D, v2f_TexCoords);
}