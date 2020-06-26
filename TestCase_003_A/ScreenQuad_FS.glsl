#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_Texture2D;

void main()
{
	vec3 TexelColor = texture(u_Texture2D, v2f_TexCoords).rgb;
	TexelColor = pow(TexelColor, vec3(1.0f/2.2f));
	Color_ = vec4(TexelColor, 1.0f);
}