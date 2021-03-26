#version 430 core

layout (location = 0) in vec2 _Position;
layout (location = 1) in vec2 _TexCoords;

layout (std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform mat4 u_ModelMatrix;

out vec3 v2f_FragPosInWorldSpace;
out vec2 v2f_TexCoords;

void main()
{
	//vec4 FragPosInWorldSpace = u_ModelMatrix * vec4(_Position, 0.0, 1.0);
	//v2f_FragPosInWorldSpace = vec3(FragPosInWorldSpace);
	//gl_Position = u_ProjectionMatrix * u_ViewMatrix * FragPosInWorldSpace;
	//v2f_TexCoords = _TexCoords;

	vec4 FragPosInWorldSpace = u_ModelMatrix * vec4(_Position, 0.0, 1.0);
	vec4 FragPosInRightCoordiante = vec4(FragPosInWorldSpace.x, FragPosInWorldSpace.y, FragPosInWorldSpace.z, 1.0);
	v2f_FragPosInWorldSpace = vec3(FragPosInRightCoordiante);
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * FragPosInWorldSpace;
	v2f_TexCoords = _TexCoords;
}