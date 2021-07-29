#version 430 core
layout(location = 0) in vec3 _Position;
layout(location = 1) in vec3 _Normal;
layout(location = 2) in vec2 _TexCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_LightVPMatrix;
void main()
{
	gl_Position = u_LightVPMatrix * u_ModelMatrix * vec4(_Position, 1.0f);
}