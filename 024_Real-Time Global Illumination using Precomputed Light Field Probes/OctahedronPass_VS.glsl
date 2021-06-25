#version 430 core

layout (location = 0) in vec2 _Position;
layout (location = 1) in vec2 _TexCoords;

out vec2 v2f_TexCoords;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

void main()
{
	gl_Position = vec4(_Position, 0.0, 1.0);
	v2f_TexCoords = _TexCoords;
}