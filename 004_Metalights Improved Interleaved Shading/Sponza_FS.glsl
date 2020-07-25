#version 430 core

in  vec3 v2f_FragPosInViewSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;

layout (location = 0) out vec3 Albedo_;
layout (location = 1) out vec3 Normal_;
layout (location = 2) out vec3 Position_;

uniform sampler2D u_DiffuseTexture;

void main()
{
	vec3 Albedo = texture(u_DiffuseTexture, v2f_TexCoords).rgb;
	Albedo_ = Albedo;
	Normal_ = v2f_Normal;
	Position_ = v2f_FragPosInViewSpace;
}