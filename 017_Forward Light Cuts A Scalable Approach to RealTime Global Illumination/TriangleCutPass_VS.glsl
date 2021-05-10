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
out VS_GS_VEXTEX
{
	 vec4 PositionS;
	 vec3 NormalW;
	 vec2 TexCoord;
}vs_gs_out;

void main()
{
	vec4 FragPos =  u_ModelMatrix * vec4(_Position, 1.0f);
	gl_Position = vec4(0.5,0.5,0.5,1);
	vs_gs_out.TexCoord = _TexCoord;
	vs_gs_out.NormalW = _Normal;	
	vs_gs_out.PositionS = u_ModelMatrix * FragPos;
}