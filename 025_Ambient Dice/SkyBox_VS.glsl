#version 430 core
layout(location = 0) in vec3 _Position;
layout(location = 1) in vec3 _Normal;
layout(location = 2) in vec2 _TexCoord;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

out vec3 v2f_TexCoords;

void main()
{
	vec4 pos = u_ProjectionMatrix * mat4(mat3(u_ViewMatrix)) * vec4(_Position, 1.0f);
    gl_Position = pos.xyww;
	v2f_TexCoords = _Position;
}