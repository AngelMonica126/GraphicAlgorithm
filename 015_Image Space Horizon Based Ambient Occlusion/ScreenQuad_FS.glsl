#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_HBAOTexture;
uniform sampler2D u_Albedo;

void main()
{
	float Ambient = (texture(u_HBAOTexture, v2f_TexCoords, 0).r);
	vec3 Albedo = texture(u_Albedo, v2f_TexCoords).rgb;
	Color_ = vec4( Albedo * Ambient,1);
}