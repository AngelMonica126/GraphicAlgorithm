#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_DirectTexture;
uniform sampler2D u_Albedo;

void main()
{
	vec3 Direct = (texture(u_DirectTexture, v2f_TexCoords, 0).rgb);
	vec3 Albedo = texture(u_Albedo, v2f_TexCoords).rgb;
	Color_ = vec4(Albedo * Direct + Albedo,1);
}
