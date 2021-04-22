#version 430 core

in  vec3 v2f_FragPosInViewSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;

layout (location = 0) out vec3 Albedo_;
layout (location = 1) out vec3 Normal_;
layout (location = 2) out vec4 Position_;

uniform sampler2D u_DiffuseTexture;
uniform vec4 u_DiffuseColor;
uniform float u_Near = 0.1;
uniform float u_Far = 1000.0f;
uniform vec3 u_LightPos = vec3(-18.5264, 19.4874, 78.5421);
layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

float LinearizeDepth(float vDepth)
{
    float z = vDepth * 2.0 - 1.0; 
    return (2.0 * u_Near * u_Far) / (u_Far + u_Near - z * (u_Far - u_Near));    
}
void main()
{
	Position_ = vec4(v2f_FragPosInViewSpace.xyz,LinearizeDepth(gl_FragCoord.z));
	Normal_ = normalize(v2f_Normal);
	vec3 LightPosInViewSpace = vec3(u_ViewMatrix * vec4(u_LightPos,1));
	vec3 Dis = LightPosInViewSpace - Position_.xyz;
	Albedo_ = texture(u_DiffuseTexture, v2f_TexCoords).xyz * max(dot(Normal_,normalize(Dis)),0) / dot(Dis,Dis) * 50;
}