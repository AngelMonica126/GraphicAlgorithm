#version 430 core

layout(location = 0) in vec3 _Position;
layout(location = 1) in vec3 _Normal;
layout(location = 2) in vec2 _TexCoord;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform mat4 u_ModelMatrix;
out vec3 v2f_FragPosInViewSpace;
out vec2 v2f_TexCoords;
out vec3 v2f_Normal;
void main()
{
	vec4 FragPosInViewSpace =  u_ModelMatrix * vec4(_Position, 1.0f);
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * FragPosInViewSpace;
	v2f_TexCoords = _TexCoord;
	v2f_Normal = _Normal;	
	v2f_FragPosInViewSpace = vec3(_Position);
}