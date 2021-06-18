#version 430 core

in  vec3 v2f_FragPosInViewSpace;
//in  vec3 v2f_Normal;

layout(location = 0) out vec4 Color_;

uniform vec3 u_DiffuseColor;

void main()
{
	vec3 Albedo = u_DiffuseColor;
	Color_ = vec4(1.0,0,0, 0.0f);
}