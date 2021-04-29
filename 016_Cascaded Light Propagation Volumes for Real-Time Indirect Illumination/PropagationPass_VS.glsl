#version 430 core

layout (location = 0) in vec3 _Position;
flat out ivec3 v2f_volumeCellIndex;

void main()
{
	v2f_volumeCellIndex = ivec3(_Position);
	gl_Position = vec4(0,0,0,1.0);
}