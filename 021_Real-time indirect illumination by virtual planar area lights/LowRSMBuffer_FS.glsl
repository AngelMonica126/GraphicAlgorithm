#version 430 core

in  vec3 v2f_WorldPos;

layout (location = 0) out vec4 Position_;

void main()
{
	Position_ = vec4(v2f_WorldPos, 1.0f);
}