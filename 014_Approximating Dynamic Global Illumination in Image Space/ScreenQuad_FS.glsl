#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_SSAOTexture;
uniform sampler2D u_Albedo;
uniform sampler2D u_SSDOTexture;
uniform sampler2D u_DirectLightSSDOTexture;

void main()
{
	float Ambient = (texture(u_SSAOTexture, v2f_TexCoords, 0).r);
	vec3 Albedo = texture(u_Albedo, v2f_TexCoords).rgb;
	Color_ = vec4(Albedo * Ambient + texture(u_SSDOTexture, v2f_TexCoords).rgb * 2,1);
}