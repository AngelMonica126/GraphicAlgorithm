#version 430 core

in  vec3 v2f_FragPosInViewSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;
layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};
layout (location = 0) out vec4 Albedo_;

uniform vec3 u_DiffuseColor;
uniform vec3 u_LightDir = vec3( 0.146059, -0.594963, -0.790371);
void main()
{	
	if((abs(v2f_Normal.x) < 0.0001f) && (abs(v2f_Normal.y) < 0.0001f) && (abs(v2f_Normal.z) < 0.0001f))
	{
		Albedo_ = vec4(0, 0, 0, 1);
		return;
	}
	vec3 Normal = normalize(v2f_Normal);
	vec3 LightDirInViewSpace = -mat3(u_ViewMatrix) * u_LightDir;
	vec3 DiffuseColor = u_DiffuseColor;
	vec3 Cool = vec3(0,0,0.55) + 0.25 * DiffuseColor;
	vec3 Warm = vec3(0.3,0.3,0) + 0.5 * DiffuseColor;
	vec3 HightLight = vec3(1);
	float t = (1 + max(0.0,dot(LightDirInViewSpace,v2f_Normal.xyz))) / 2;
	vec3 R = 2 * max(0.0,dot(LightDirInViewSpace,v2f_Normal.xyz)) * v2f_Normal.xyz - LightDirInViewSpace;
	vec3 ViewDir = -normalize(v2f_FragPosInViewSpace);
	float s = clamp(100 * max(0.0,dot(R,ViewDir)) - 97,0,1);
	Albedo_ = vec4(s * HightLight + (1 - s) * (t * Warm + (1 - t) * Cool),1.0);
}