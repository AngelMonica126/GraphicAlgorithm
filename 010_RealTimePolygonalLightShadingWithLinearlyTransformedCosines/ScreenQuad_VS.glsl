#version 430 core

layout (location = 0) in vec2 _Position;
layout (location = 1) in vec2 _TexCoord;

out vec2 v2f_TexCoord;

void main()
{
	gl_Position = vec4(_Position, 0.0, 1.0);
	v2f_TexCoord = _TexCoord;
}