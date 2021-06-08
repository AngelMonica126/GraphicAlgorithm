#version 430 core
in vec2 test;
layout (location = 0) out vec2 Depth_;	

uniform vec3  u_LightColor = vec3(1);

void main()
{
    float depth = (gl_FragCoord.z);
	Depth_ = vec2(depth,depth * depth);
}