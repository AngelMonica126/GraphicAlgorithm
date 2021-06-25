#version 430 core
layout(location = 0) in vec3 _Position;
layout(location = 1) in vec3 _Normal;
layout(location = 2) in vec2 _TexCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_LightVPMatrix;

out vec2 v2f_TexCoords;
out vec3 v2f_WorldNormal;
out vec3 v2f_WorldPos;

void main()
{
	vec4 FragPosInWorldSpace = u_ModelMatrix * vec4(_Position, 1.0f);
	gl_Position = u_LightVPMatrix * FragPosInWorldSpace;
	v2f_TexCoords = _TexCoord;
	v2f_WorldNormal = _Normal;	
	v2f_WorldPos = vec3(FragPosInWorldSpace);
}