#version 430 core

in  vec3 v2f_FragPosInViewSpace;
in  vec3 v2f_Normal;

layout(location = 0) out vec4 AlbedoAndMetallic_;
layout(location = 1) out vec4 NormalAndDoubleRoughness_;
layout(location = 2) out vec4 Position_;

uniform vec3 u_DiffuseColor;

void main()
{
	vec3 Albedo = u_DiffuseColor;
	AlbedoAndMetallic_ = vec4(u_DiffuseColor, 0.0f);
	vec3 Normal = normalize(v2f_Normal);
	NormalAndDoubleRoughness_ = vec4(Normal, 0);
	Position_ = vec4(v2f_FragPosInViewSpace, 1.0f);
}