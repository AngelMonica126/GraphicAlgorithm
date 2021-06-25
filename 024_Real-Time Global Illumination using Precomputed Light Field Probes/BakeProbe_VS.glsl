#version 430 core
layout(location = 0) in vec3 _Position;
layout(location = 1) in vec3 _Normal;
layout(location = 2) in vec2 _TexCoord;

uniform mat4 u_BakeProjectionMatrix;
uniform mat4 u_BakeViewMatrix;
uniform mat4 u_ModelMatrix;
out vec3 v2f_FragPosInViewSpace;
out vec2 v2f_TexCoords;
out vec3 v2f_Normal;
void main()
{
	vec4 FragPosInViewSpace = u_BakeViewMatrix * u_ModelMatrix * vec4(_Position, 1.0f);
	gl_Position = u_BakeProjectionMatrix * FragPosInViewSpace;
	v2f_TexCoords = _TexCoord;
	v2f_Normal =  normalize(mat3(transpose(inverse(u_BakeViewMatrix * u_ModelMatrix))) * _Normal);	
	v2f_FragPosInViewSpace = vec3(FragPosInViewSpace);
}