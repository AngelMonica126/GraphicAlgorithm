#version 430 core
layout(location = 0) in vec3 _Position;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform mat4 u_ModelMatrix;

out vec3 v2f_FragPosInWorldSpace;
void main()
{
	vec4 FragPosInWorldSpace = u_ModelMatrix * vec4(_Position, 1.0f);
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * FragPosInWorldSpace;
	v2f_FragPosInWorldSpace = vec3(FragPosInWorldSpace);
}