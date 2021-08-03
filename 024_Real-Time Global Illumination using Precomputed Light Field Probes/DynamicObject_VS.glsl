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

out vec3 v2f_Normal;
out vec3 v2f_FragPosInWorldSpace;
void main()
{
	vec4 FragPosInWorldSpace =  u_ModelMatrix * vec4(_Position, 1.0f);
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * FragPosInWorldSpace;
	v2f_Normal = (mat3(u_ModelMatrix) * _Normal);
	v2f_FragPosInWorldSpace = vec3(FragPosInWorldSpace);
}